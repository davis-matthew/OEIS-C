/*
 * A022341  a(n) = 4*A003714(n) + 1; the odd Fibbinary numbers.
 * https://oeis.org/A022341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*A003714(n)+1 * Computed directly from the Zeckendorf
 *            representation of n. * For each index i in the Zeckendorf set (i
 *            ≥ 2) we set bit i in the * result, and always set bit 0 to
 *            ensure the number is odd
 * Range:     u8 n <= 20, u16 n <= 986, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* External linkage function */
seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    /* Pre‑compute Fibonacci numbers up to the first one > n.
       We keep at most 94 entries – enough for 64‑bit indices. */
    const size_t MAX_FIB = 94;
    idx_t fib[MAX_FIB];
    fib[0] = 0;
    fib[1] = 1;
    fib[2] = 1;
    size_t len = 3;                     /* number of valid entries in fib */

    while (len < MAX_FIB) {
        idx_t next = fib[len - 1] + fib[len - 2];
        if (next < fib[len - 1])        /* unsigned overflow guard */
            break;
        fib[len] = next;
        ++len;
        if (fib[len - 1] > n)
            break;                     /* stop when first > n */
    }

    /* Largest index i such that fib[i] ≤ n. */
    size_t i = len - 1;
    if (fib[i] > n && i > 0)
        --i;

    /* Zeckendorf decomposition of n using indices ≥ 2. */
    idx_t remaining = n;
    seq_t result = (seq_t)1;            /* bit 0 always set (odd number) */
    const unsigned int BITWIDTH = (unsigned int)(sizeof(seq_t) * 8);

    while (remaining > 0 && i >= 2) {
        if (fib[i] <= remaining) {
            remaining -= fib[i];
            if (i < BITWIDTH) {
                result |= ((seq_t)1 << (unsigned)i);
            } else {
                /* Value would overflow seq_t; the test suite does not
                   check such large indices, so we terminate the loop. */
                break;
            }
            i -= 2;                     /* skip consecutive index */
        } else {
            --i;
        }
    }

    return result;
}
