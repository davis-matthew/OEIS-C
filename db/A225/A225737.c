/*
 * A225737  Numbers k such that k < d(k)^(29/10), where d(k) is the number of
 *          divisors of k.
 * https://oeis.org/A225737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the n-th integer k such that k^10 < d(k)^29, where d(k)
 *            is the divisor count (k > 0). The sequence begins 2, 3, 4, 5, 6,
 *            7, 8, 9, 10, 12, 14, …
 * Range:     u8 n <= 130, u16 n <= 3826, u32 n <= 9728, u64 n <= 9728,
 *            u128 n <= 9728, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
/* divisor_count_u64 is supplied by the utilities library. */
#include "../utils/divisor_count_u64.h"

typedef struct { uint64_t v[4]; } Uint256;     /* 256‑bit accumulator */

/* multiply a 256‑bit number by a 64‑bit integer */
static Uint256 mul_uint256_uint64(const Uint256 *a, uint64_t b)
{
    Uint256 r = {0};
    unsigned __int128 carry = 0;
    for (int i = 0; i < 4; ++i) {
        unsigned __int128 prod = (unsigned __int128)a->v[i] * b + carry;
        r.v[i] = (uint64_t)prod;
        carry  = prod >> 64;
    }
    return r;
}

/* compute base^exp (exp <= 30) in 256‑bit */
static Uint256 pow_uint256_uint64(uint64_t base, unsigned int exp)
{
    Uint256 res = {0}; res.v[0] = 1;
    for (unsigned int i = 0; i < exp; ++i)
        res = mul_uint256_uint64(&res, base);
    return res;
}

/* compare two 256‑bit numbers: <0 if a<b, 0 if a==b, >0 if a>b */
static int cmp_uint256(const Uint256 *a, const Uint256 *b)
{
    for (int i = 3; i >= 0; --i) {
        if (a->v[i] < b->v[i]) return -1;
        if (a->v[i] > b->v[i]) return 1;
    }
    return 0;
}

/* decide whether k is in the sequence */
static bool in_sequence_uint64(uint64_t k)
{
    uint64_t d = divisor_count_u64(k);
    Uint256 left  = pow_uint256_uint64(k, 10);   /* k^10 */
    Uint256 right = pow_uint256_uint64(d, 29);   /* d^29 */
    return cmp_uint256(&left, &right) < 0;       /* left < right */
}

/* static state for memoised terms */
static uint64_t *k_terms = NULL;    /* store the found k's as uint64_t */
static size_t k_cap = 0;            /* current capacity */
static size_t k_count = 0;          /* number of stored terms */
static uint64_t last_checked = 1;   /* last integer examined */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined at offset 0 */

    /* initialise the static table on the first call */
    if (k_terms == NULL) {
        k_cap = 8;
        k_terms = (uint64_t*)malloc(k_cap * sizeof(uint64_t));
        if (k_terms == NULL) return 0;   /* allocation failure */
    }

    /* generate terms until we have the first n of them */
    while (k_count < n) {
        ++last_checked;
        if (in_sequence_uint64(last_checked)) {
            if (k_count == k_cap) {
                size_t new_cap = k_cap * 2;
                uint64_t *tmp = (uint64_t*)realloc(k_terms, new_cap * sizeof(uint64_t));
                if (tmp == NULL) return 0;   /* allocation failure */
                k_terms = tmp;
                k_cap   = new_cap;
            }
            k_terms[k_count++] = last_checked;
        }
    }

    /* return the n‑th term; cast to seq_t (overflow is permitted for small seq_t) */
    return (seq_t)k_terms[n - 1];
}
