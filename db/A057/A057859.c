/*
 * A057859  Number of residue classes modulo n which contain a prime.
 * https://oeis.org/A057859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = φ(n) + ω(n) (Euler's totient + number of distinct primes)
 * Range:     u8 n <= 69, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   allocate_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined for n ≥ 1.  It is computed by sieving φ and ω up
 * to the largest index requested.  The implementation uses dynamic
 * allocation that works for both C and C++ (seq_t can be an
 * arbitrary‑precision integer class or an unsigned integer).  The
 * allocator calls constructors in C++ and uses malloc/free in C.
 */

#include <stdlib.h>
#include <assert.h>
#include "../utils/allocate_seq_array.h"

static seq_t *phi = NULL;   /* φ(i) for 0 ≤ i < cap   */
static seq_t *omega = NULL; /* ω(i) for 0 ≤ i < cap   */
static size_t cap = 0;      /* current maximum index + 1 */

static void free_seq_array(seq_t *p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* Compute φ and ω up to at least n (inclusive). */
static void recompute_up_to(idx_t n)
{
    size_t N = (size_t)n;
    if (cap > N) return;          /* already computed enough */

    size_t new_cap = N + 1;
    seq_t *new_phi   = allocate_seq_array(new_cap);
    seq_t *new_omega = allocate_seq_array(new_cap);
    if (!new_phi || !new_omega) abort(); /* allocation failed */

    /* Initialize: φ(i)=i, ω(i)=0  (φ[0] unused, φ[1] will stay 1) */
    for (size_t i = 0; i < new_cap; ++i) {
        new_phi[i]   = (seq_t)i;
        new_omega[i] = (seq_t)0;
    }
    if (new_cap > 1) new_phi[1] = (seq_t)1;

    /* Sieve to compute φ and ω */
    for (size_t i = 2; i < new_cap; ++i) {
        if (new_phi[i] == (seq_t)i) {  /* i is prime */
            for (size_t j = i; j < new_cap; j += i) {
                new_phi[j]   -= new_phi[j] / (seq_t)i;
                new_omega[j] += (seq_t)1;
            }
        }
    }

    /* Replace old arrays */
    free_seq_array(phi);
    free_seq_array(omega);
    phi = new_phi;
    omega = new_omega;
    cap = new_cap;
}

/* OEIS sequence a(n): number of residue classes modulo n that contain a prime */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* safety, though offset is 1 */
    recompute_up_to(n);
    return phi[(size_t)n] + omega[(size_t)n];
}
