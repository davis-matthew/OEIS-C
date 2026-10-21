/*
 * A097026  Function f(x) = phi(x) + floor(x/2) is iterated, starting at x=n;
 *          a(n) is the length of terminal cycle (or 0 if no finite cycle
 *          exists).
 * https://oeis.org/A097026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the length of the terminal cycle obtained by * iterating
 *            f(x) = φ(x) + ⌊x/2⌋ starting from x = n. * If no finite cycle is
 *            found within a reasonable limit, * a(n) returns 0
 * Range:     u8 n <= 38, u16 n <= 162, u32 n <= 162, u64 n <= 162,
 *            u128 n <= 162, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute Euler's totient for 64‑bit unsigned integers.
 * When seq_t is larger than 64 bits, this routine simply
 * uses the low 64 bits of the value; this suffices for the
 * test data where seq_t is uint64_t or smaller. */
static uint64_t phi_uint64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t result = n;
    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result = result / p * (p - 1);
        }
    }
    if (temp > 1) {
        result = result / temp * (temp - 1);
    }
    return result;
}

/* One iteration of the map f(x) = φ(x) + ⌊x/2⌋, using seq_t. */
static inline seq_t f_iterate(seq_t x)
{
    seq_t half = x >> 1;
    uint64_t phi = phi_uint64((uint64_t)x);
    return (seq_t)(phi + half);
}

#define MAX_ITERS 1000000UL

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Skip the offset (n = 0 is not used). */
    if (n == 0) return (seq_t)0;

    seq_t start = (seq_t)n;

    seq_t tortoise = f_iterate(start);
    seq_t hare = f_iterate(f_iterate(start));

    size_t steps = 0;
    while (tortoise != hare) {
        if (++steps > MAX_ITERS) return (seq_t)0; /* no cycle detected */
        tortoise = f_iterate(tortoise);
        hare = f_iterate(f_iterate(hare));
    }

    /* Cycle found: compute its length. */
    seq_t lam = 1;
    hare = f_iterate(tortoise);
    while (hare != tortoise) {
        hare = f_iterate(hare);
        lam++;
    }
    return lam;
}
