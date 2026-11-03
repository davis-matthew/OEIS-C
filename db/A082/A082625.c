/*
 * A082625  Smallest palindromic prime that ends (on the least significant
 *          side) in prime(n).
 * https://oeis.org/A082625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   the smallest palindromic prime that ends (on the least
 *            significant side) in the n-th prime
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits0, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* utilities */
#include <math.h>
#include "../utils/num_digits0.h"
#include "../utils/reverse_digits.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ------------------------------------------------------------ */
/* prime generation -------------------------------------------------- */
static uint64_t *prime_list = NULL;
static size_t prime_count  = 0;
static size_t prime_cap    = 0;

/* ensure at least `need` primes are stored in prime_list */
static void ensure_primes(size_t need)
{
    if (prime_count >= need)
        return;

    /* grow buffer if necessary */
    if (prime_count == 0) {
        prime_cap = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_list) exit(EXIT_FAILURE);
        prime_list[0] = 2;
        prime_count = 1;
    }
    if (prime_cap < need)
        prime_cap = need;
    prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
    if (!prime_list) exit(EXIT_FAILURE);

    uint64_t cand;
    if (prime_count == 1)
        cand = 3;
    else
        cand = prime_list[prime_count - 1] + 2;   /* only odd numbers */

    while (prime_count < need) {
        bool ok = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p > cand / p) /* p*p would overflow */
                break;
            if (cand % p == 0) {
                ok = false;
                break;
            }
        }
        if (ok) {
            prime_list[prime_count++] = cand;
        }
        cand += 2;
    }
}

static uint64_t get_prime(idx_t n)
{
    ensure_primes((size_t)n);
    return prime_list[(size_t)(n - 1)];
}

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/* main sequence function -------------------------------------- */
seq_t a(idx_t n)
{
    /* ----- 1.  obtain the n‑th prime and related values ---------- */
    uint64_t p_uint = get_prime(n);
    seq_t S = (seq_t)p_uint;
    uint64_t k_uint = num_digits0(p_uint);
    seq_t rev = reverse_digits(S);
    seq_t pow10k = pow10((idx_t)k_uint);

    /* ----- 2.  check if the prime itself is a palindrome prime ----- */
    if (rev == S && is_prime_big(S))
        return S;

    /* ----- 3.  search palindromes of increasing total length ------ */
    /* Let L be the length of the palindrome.  We start from
       L = k (the smallest possible length).  For each L we generate
       all palindromes of length L whose last k digits equal the prime. */
    seq_t pow10k_val = pow10k;   /* keep for modulus */
    for (uint64_t L = k_uint; ; ++L) {
        uint64_t lh = (L + 1) / 2;          /* length of left half */
        seq_t pow10_lh   = pow10((idx_t)lh);
        seq_t pow10_lh1  = pow10((idx_t)(lh > 0 ? lh - 1 : 0));

        /* iterate all left halves with non‑zero most significant digit */
        uint64_t start = 1;
        for (uint64_t i = 1; i < lh; ++i)
            start *= 10;              /* 10^(lh-1) */
        uint64_t end   = start * 10;  /* 10^lh */

        for (uint64_t leftHalf = start; leftHalf < end; ++leftHalf) {
            seq_t palindrome;
            if (L % 2 == 0) {
                /* even length: palindrome = leftHalf || reverse(leftHalf) */
                palindrome = (seq_t)leftHalf * pow10_lh + reverse_digits((seq_t)leftHalf);
            } else {
                /* odd length: palindrome = leftHalf || reverse(leftHalf/10) */
                palindrome = (seq_t)leftHalf * pow10_lh1 + reverse_digits((seq_t)(leftHalf / 10));
            }

            /* check suffix condition */
            if (palindrome % pow10k_val != S)
                continue;

            /* prime test */
            if (is_prime_big(palindrome))
                return palindrome;
        }
    }
}
