/*
 * A066594  Primes which can be expressed as concatenation of powers of 3 and
 *          0's.
 * https://oeis.org/A066594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the nth prime p that can be written as a concatenation of
 *            powers of 3 (including 3⁰ = 1) and optional 0's
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/num_digits.h"
#include "../utils/pow3.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

/* ----- helpers for powers of 3 ----- */
static char **pow_strs = NULL;      /* decimal strings of powers of 3      */
static size_t *pow_lens = NULL;    /* their lengths (in digits)           */
static size_t pow_cap  = 0;        /* allocated capacity                  */
static size_t pow_cnt  = 0;        /* number of stored powers             */
static size_t pow_max_len = 0;     /* maximum token length seen so far    */
static size_t pow_exp = 0;         /* next exponent to compute           */

static void ensure_powers_len(size_t needed_len)
{
    /* generate powers of 3 whose decimal length ≤ needed_len */
    while (true) {
        seq_t val = pow3((idx_t)pow_exp);     /* compute next power     */
        size_t dlen = num_digits(val);         /* its number of digits   */
        if (dlen > needed_len) break;         /* stop once too long    */

        /* Allocate space for strings if necessary */
        if (pow_cnt == pow_cap) {
            size_t new_cap = pow_cap == 0 ? 8 : pow_cap * 2;
            char **new_strs = (char **)realloc(pow_strs,
                                                new_cap * sizeof(char *));
            pow_strs = new_strs;
            size_t *new_lens = (size_t *)realloc(pow_lens,
                                                  new_cap * sizeof(size_t));
            pow_lens = new_lens;
            pow_cap = new_cap;
        }

        /* Convert to string */
        char *s = (char *)malloc(dlen + 1);
        size_t outlen;
        seq_to_str(val, s, &outlen); /* assume outlen == dlen   */
        s[dlen] = '\0';

        pow_strs[pow_cnt] = s;
        pow_lens[pow_cnt] = dlen;
        ++pow_cnt;
        if (dlen > pow_max_len) pow_max_len = dlen;
        ++pow_exp;
    }
}

/* ----- test if a prime can be written as concatenation of powers of 3 or
   0's ----- */
static bool valid_prime(seq_t val)
{
    size_t len = num_digits(val);
    char *s = (char *)malloc(len + 1);
    size_t outlen;
    seq_to_str(val, s, &outlen);
    s[len] = '\0';

    bool *dp = (bool *)malloc((len + 1) * sizeof(bool));
    memset(dp, 0, (len + 1) * sizeof(bool));
    dp[0] = true;

    for (size_t i = 0; i < len; ++i) {
        if (!dp[i]) continue;

        /* single zero token */
        if (s[i] == '0')
            dp[i + 1] = true;

        /* try all stored powers of 3 */
        for (size_t k = 0; k < pow_cnt; ++k) {
            size_t l = pow_lens[k];
            if (i + l > len) break;
            if (strncmp(s + i, pow_strs[k], l) == 0)
                dp[i + l] = true;
        }
    }

    bool ok = dp[len];
    free(dp);
    free(s);
    return ok;
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1 */

    /* static cache of found terms */
    static seq_t *terms   = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;

    /* next candidate number to test (works for both integral and big‑int seq_t) */
    static seq_t next_candidate = 2;

    while (terms_len < n) {
        /* find next prime that satisfies the property */
        while (true) {
            if (is_prime_big(next_candidate)) {
                /* ensure powers list covers this prime's length */
                size_t needed = num_digits(next_candidate);
                ensure_powers_len(needed);

                if (valid_prime(next_candidate)) {
                    /* store term */
                    if (terms_len == terms_cap) {
                        size_t new_cap = terms_cap == 0 ? 8 : terms_cap * 2;
                        seq_t *new_arr = mem_resize(seq_t, terms, new_cap);
                        terms = new_arr;
                        terms_cap = new_cap;
                    }
                    terms[terms_len++] = next_candidate;
                    /* advance to next candidate for future searches */
                    next_candidate = next_candidate + 1;
                    break;  /* move to next candidate after storing */
                }
            }
            next_candidate = next_candidate + 1;
        }
    }

    return terms[n - 1];
}
