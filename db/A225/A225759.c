/*
 * A225759  Primes p such that A179382((p+1)/2) = (p-1)/16.
 * https://oeis.org/A225759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over primes p == 1 (mod 16): a(n) is the n-th prime with
 *            ord_p(2) = (p-1)/8. (A179382((p+1)/2) counts the odd elements of
 *            the subgroup <2> mod p. If ord_p(2) is even, -1 is in <2>, so
 *            exactly half the elements are odd and A179382((p+1)/2) =
 *            ord_p(2)/2. If ord_p(2) is odd, the count cannot...
 * Range:     u8 none, u16 n <= 86, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 55 terms through n = 9851 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static unsigned char *A225759_comp = 0;   /* composite flags, index = number */
static uint64_t A225759_lim = 0;
static uint64_t *A225759_terms = 0;
static size_t A225759_cnt = 0, A225759_cap = 0;
static uint64_t A225759_next = 17;

static void A225759_sieve(uint64_t lim)
{
    free(A225759_comp);
    A225759_comp = (unsigned char *)malloc((size_t)lim + 1);
    memset(A225759_comp, 0, (size_t)lim + 1);
    A225759_comp[0] = A225759_comp[1] = 1;
    for (uint64_t i = 2; i * i <= lim; i++)
        if (!A225759_comp[i])
            for (uint64_t j = i * i; j <= lim; j += i) A225759_comp[j] = 1;
    A225759_lim = lim;
}

static uint64_t A225759_pw2(uint64_t e, uint64_t p)
{
    unsigned __int128 r = 1, b = 2 % p;
    while (e) {
        if (e & 1) r = r * b % p;
        b = b * b % p;
        e >>= 1;
    }
    return (uint64_t)r;
}

static bool A225759_check(uint64_t p)
{
    uint64_t t = (p - 1) / 8;
    if (A225759_pw2(t, p) != 1) return false;
    uint64_t m = t;
    for (uint64_t q = 2; q * q <= m; q++) {
        if (m % q) continue;
        while (m % q == 0) m /= q;
        if (A225759_pw2(t / q, p) == 1) return false;
    }
    if (m > 1 && A225759_pw2(t / m, p) == 1) return false;
    return true;
}

seq_t a(idx_t n)
{
    while (A225759_cnt < (size_t)n) {
        uint64_t p = A225759_next;
        if (p > A225759_lim)
            A225759_sieve(A225759_lim ? A225759_lim * 2 : (1u << 20));
        A225759_next += 16;
        if (A225759_comp[p]) continue;
        if (!A225759_check(p)) continue;
        if (A225759_cnt == A225759_cap) {
            A225759_cap = A225759_cap ? A225759_cap * 2 : 256;
            A225759_terms = (uint64_t *)realloc(A225759_terms, A225759_cap * sizeof(uint64_t));
        }
        A225759_terms[A225759_cnt++] = p;
    }
    return (seq_t)A225759_terms[n - 1];
}
