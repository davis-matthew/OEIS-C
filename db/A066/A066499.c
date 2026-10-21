/*
 * A066499  Numbers k such that phi(k) == 2 (mod 4).
 * https://oeis.org/A066499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k > 0 such that φ(k) ≡ 2 (mod 4), where
 *            φ is Euler's totient function. The sequence is {3, 4, 6, 7, 9,
 *            11, 14, 18, 19, …}
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, phi_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation scans integers sequentially, testing the
   totient condition with the helper phi_local, and stores terms
   in a dynamic array so that subsequent calls do not repeat work. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/phi_local.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* static storage for already found terms */
    static seq_t *terms = NULL;
    static size_t len = 0, cap = 0;

    /* last integer that has been tested */
    static idx_t tested = 2;

    /* keep generating terms until we have at least n of them */
    while (len < n)
    {
        tested++;                      /* next integer to test */
        seq_t ph = phi_local((seq_t)tested);
        if ((ph & 3U) == 2)            /* φ(tested) ≡ 2 (mod 4) ? */
            append_candidate(&terms, &len, &cap, (seq_t)tested);
    }

    return terms[n - 1];               /* 1‑based indexing */
}
