/*
 * A088270  Palindromic primes that yield a prime when sandwiched between two
 *          3's. (Prefixing and suffixing a -three' on both sides yields
 *          another pal prime).
 * https://oeis.org/A088270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth palindromic prime p such that 3p3 is also a
 *            palindromic prime. The first terms are 5, 7, 101, 151, 353, 727,
 *            757, 929, 10301, …
 * Range:     u8 none, u16 n <= 8, u32 n <= 970, u64 n <= 5000, u128 n <= 5000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- basic utilities ---------- */
static inline int digit_len(seq_t x) {
    int d = 0;
    do { d++; x /= 10; } while (x);
    return d;
}

/* Miller‑Rabin primality test for 64‑bit integers */
static inline uint64_t mulmod(uint64_t a, uint64_t b, uint64_t m) {
    return (unsigned __int128)a * b % m;
}
static inline uint64_t powmod(uint64_t a, uint64_t e, uint64_t m) {
    uint64_t res = 1 % m;
    while (e) {
        if (e & 1) res = mulmod(res, a, m);
        a = mulmod(a, a, m);
        e >>= 1;
    }
    return res;
}
static bool isPrime64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t smallp[] = {
        2,3,5,7,11,13,17,19,23,29,31,37,41,43,47
    };
    for (size_t i = 0; i < sizeof smallp / sizeof *smallp; ++i) {
        if (n % smallp[i] == 0) return n == smallp[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = powmod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mulmod(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* ---------- palindrome generator ---------- */
static seq_t pow10_tbl[40];
static bool pow10_inited = false;

static void init_pow10(void) {
    pow10_tbl[0] = 1;
    for (int i = 1; i < 40; ++i) pow10_tbl[i] = pow10_tbl[i-1] * 10;
    pow10_inited = true;
}

/* static state for the palindrome generator */
static int  g_len       = 0;      /* current palindrome length   */
static int  g_half_len  = 0;      /* half‑length used for generation */
static seq_t g_half     = 0;      /* current half value          */
static seq_t g_start    = 0;      /* start half                  */
static seq_t g_end      = 0;      /* inclusive end half          */
static bool  g_first    = true;   /* first call ?                */

static seq_t next_pal(void) {
    if (g_first) {                    /* first initialisation */
        g_len = 1;
        g_half_len = 1;
        g_start = 1;
        g_end   = 9;
        g_half  = g_start;
        g_first = false;
    }

    for (;;) {
        /* build the palindrome from the current half */
        seq_t half = g_half;
        seq_t rev  = 0;
        seq_t tmp  = half;
        if (g_len & 1) tmp /= 10;        /* skip middle digit for odd length */
        while (tmp) {
            rev = rev * 10 + tmp % 10;
            tmp /= 10;
        }
        seq_t pal;
        if (g_len & 1) /* odd length */
            pal = half * pow10_tbl[g_half_len-1] + rev;
        else           /* even length */
            pal = half * pow10_tbl[g_half_len]   + rev;

        /* advance half for next call */
        ++g_half;
        if (g_half > g_end) {
            /* move to next palindrome length */
            ++g_len;
            g_half_len = (g_len & 1) ? (g_len + 1) / 2 : g_len / 2;
            if (g_len == 1) {
                g_start = 1;
            } else {
                g_start = pow10_tbl[g_half_len-1];
            }
            g_end = pow10_tbl[g_half_len] - 1;
            g_half = g_start;
        }
        return pal;
    }
}

/* ---------- memoised sequence storage ---------- */
static seq_t *terms      = NULL;
static size_t terms_sz  = 0;
static size_t terms_cap = 0;

static void add_term(seq_t v) {
    if (terms_sz == terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 8;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_sz++] = v;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;    /* undefined for 0, return 0 */
    /* initialise pow10 table */
    if (!pow10_inited) init_pow10();

    /* extend sequence until we have at least n terms */
    while (terms_sz < n) {
        seq_t pal = next_pal();
        /* skip even‑length palindromes that are even (not prime) quickly */
        if (!isPrime64((uint64_t)pal)) continue;

        /* compute 3p3 */
        int len = digit_len(pal);
        seq_t part = 3ULL * pow10_tbl[len+1] + pal * 10ULL + 3ULL;
        if (!isPrime64((uint64_t)part)) continue;

        /* store this palindromic prime */
        add_term(pal);
    }
    return terms[n-1];
}
