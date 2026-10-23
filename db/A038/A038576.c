/*
 * A038576  CONTINUANT transform of {phi(n)}, 1, 1, 2, 2, 4, 2, .. (A002088).
 * https://oeis.org/A038576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = continuant transform of φ(n). Recurrence: a(1) = 1 a(2) =
 *            φ(1) * φ(2) + 1 a(n) = φ(n) * a(n‑1) + a(n‑2) for n ≥ 3 where
 *            φ(k) is Euler’s totient function. The implementation precomputes
 *            φ(k) up to the maximum n requested using a sieve and stores both
 *            φ and a in static arrays, so that calls...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 39,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static idx_t max_n = 0;
static seq_t *phi_arr = NULL;   /* φ(0) unused, φ[1..max_n] stored */
static seq_t *a_arr   = NULL;   /* a[0] unused, a[1..max_n] stored */

/* Allocate or grow φ and a arrays to at least size (limit+1). */
static void init_tables(idx_t limit)
{
    if (limit <= max_n) return;

    /* Allocate new φ array */
    seq_t *new_phi = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!new_phi) exit(1);

    /* Start with φ[i] = i */
    new_phi[0] = 0;
    if (limit >= 1) new_phi[1] = (seq_t)1;
    for (idx_t i = 2; i <= limit; ++i) new_phi[i] = (seq_t)i;

    /* Sieve primes up to limit */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(limit, &primes, &primes_cnt);

    /* Apply φ formula: φ[m] -= φ[m]/p for each prime p dividing m */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes[pi];
        for (idx_t m = p; m <= limit; m += p) {
            new_phi[m] = new_phi[m] / (seq_t)p * (seq_t)(p - 1);
        }
    }
    free(primes);

    /* Replace old φ array */
    free(phi_arr);
    phi_arr = new_phi;

    /* Allocate or grow a array */
    seq_t *new_a = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!new_a) exit(1);

    if (limit >= 1) new_a[1] = (seq_t)1;
    if (limit >= 2) new_a[2] = phi_arr[1] * phi_arr[2] + (seq_t)1;

    for (idx_t i = 3; i <= limit; ++i) {
        new_a[i] = phi_arr[i] * new_a[i - 1] + new_a[i - 2];
    }

    free(a_arr);
    a_arr = new_a;

    max_n = limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    init_tables(n);
    return a_arr[n];
}
