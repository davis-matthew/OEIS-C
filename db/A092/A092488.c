/*
 * A092488  a(n) = least k such that {n+0, n+1, n+2, n+3, ... n+k} has a
 *          nonempty subset the product of whose members is a square.
 * https://oeis.org/A092488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sym_diff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A092488 – least k such that {n, n+1, … , n+k} contains a subset whose product is a perfect square.
    The algorithm builds the vectors of parity of prime exponents for the numbers in the interval.
    It adds the vectors incrementally to a basis over GF(2).  When a newly added vector reduces to zero,
    the set is linearly dependent, meaning a subset product is a square; the current distance k is returned.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/sym_diff.h"

/*------------------------------------------------------------*/
/*  Prime list generation                                        */
/*------------------------------------------------------------*/
#define MAX_PRIME_SQRT 200              /* sqrt( 20000 )  */
#define MAX_PRIME_NUM  2265             /* number of primes <= 20000 */

static int primes[MAX_PRIME_NUM];
static size_t num_primes = 0;
static bool primes_prepared = false;

static void prepare_primes(void)
{
    if (primes_prepared) return;
    bool sieve[20001] = {false};
    for (int i = 2; i * i <= 20000; ++i)
        if (!sieve[i])
            for (int j = i * i; j <= 20000; j += i)
                sieve[j] = true;
    for (int i = 2; i <= 20000; ++i)
        if (!sieve[i]) primes[num_primes++] = i;
    primes_prepared = true;
}

/*------------------------------------------------------------*/
/*  Factorisation – parity of exponents (GF(2) vector)         */
/*------------------------------------------------------------*/
static size_t factor_odd_primes(uint64_t n, int *out)
{
    size_t cnt = 0;
    for (size_t i = 0; i < num_primes && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        int p = primes[i];
        int e = 0;
        while ((n % p) == 0) {
            n /= p;
            e ^= 1;
        }
        if (e) out[cnt++] = p;
    }
    if (n > 1) out[cnt++] = (int)n;   /* n is prime > 20000 */
    return cnt;
}

/*------------------------------------------------------------*/
/*  Symmetric difference of two sorted arrays (GF(2) addition)*/

/*------------------------------------------------------------*/
/*  Basis structure over GF(2)                                 */
/*------------------------------------------------------------*/
#define MAX_BASIS 200          /* sufficient for n <= 10000 */
#define MAX_VEC   20

typedef struct {
    int data[MAX_VEC];
    size_t len;
} Vec;

static Vec basis[MAX_BASIS];
static size_t basis_cnt = 0;

/*------------------------------------------------------------*/
/*  Reduce a vector against current basis; return true if it
   becomes zero (dependent).                                     */
/*------------------------------------------------------------*/
static bool reduce_one_vec(int *vec, size_t *len)
{
    int temp[MAX_VEC];
    size_t tlen;

    for (size_t bi = 0; bi < basis_cnt; ++bi) {
        /* pivot of basis vector is its first (smallest) prime */
        int pivot = basis[bi].data[0];
        /* find pivot in vec */
        size_t idx = 0;
        while (idx < *len && vec[idx] != pivot) ++idx;
        if (idx == *len) continue;  /* pivot not present */

        /* XOR vec with basis[bi] */
        sym_diff(vec, *len, basis[bi].data, basis[bi].len, temp, &tlen);
        memcpy(vec, temp, tlen * sizeof(int));
        *len = tlen;

        if (*len == 0) return true;          /* zero vector */
    }
    return false;
}

/*------------------------------------------------------------*/
/*  Main sequence function                                      */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    prepare_primes();
    basis_cnt = 0;                /* reset for each call */

    for (uint64_t m = n; ; ++m) {
        int vec[MAX_VEC];
        size_t vlen = factor_odd_primes(m, vec);

        if (vlen == 0)                /* m is a perfect square */
            return (seq_t)(m - n);

        if (!reduce_one_vec(vec, &vlen)) {
            /* independent: add to basis */
            memcpy(basis[basis_cnt].data, vec, vlen * sizeof(int));
            basis[basis_cnt].len = vlen;
            ++basis_cnt;
        } else {
            /* dependent: found required k */
            return (seq_t)(m - n);
        }
    }
}
