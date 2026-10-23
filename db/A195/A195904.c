/*
 * A195904  Base-2 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,0,0,0,0.
 * https://oeis.org/A195904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ⌊2^(n+5)/63⌋
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 123,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"          /* seq_pow(seq_t base, idx_t exp) */

/*
 * Compute the MurmurHash2 sequence term a(n) for n ≥ 1.
 *
 * For 64‑bit builds (sizeof(seq_t) == 8) we use a recurrence that
 * only involves 64‑bit arithmetic and thus avoids overflow:
 *
 *   q_{k+1} = 2·q_k + ⌊(2·rem_k)/63⌋
 *   rem_{k+1} = (2·rem_k) % 63
 *
 * where rem_k = 2^k mod 63 and q_k = ⌊2^k/63⌋.  Starting with
 * k = 5 (2^5 = 32) gives a(1) correctly.
 *
 * For arbitrary‑precision seq_t the product 2^(n+5) is computed
 * with seq_pow and then divided by 63.
 */
seq_t a(idx_t n)
{
    idx_t exp = n + 5;   /* exponent of the power of two                */

    /* 64‑bit implementation using the recurrence described above.  */
    if (sizeof(seq_t) <= 8) {
        /* For exp = 5 the result is 0.  n ≥ 1 ⇒ exp ≥ 6.           */
        unsigned int rem = 32;          /* 2^5 mod 63                               */
        seq_t     q   = 0;              /* ⌊2^5 / 63 = 0                            */

        for (idx_t k = 5; k < exp; ++k) {
            unsigned int add = (rem * 2U) / 63U;   /* ⌊(2·rem)/63⌋      */
            rem = (rem * 2U) % 63U;               /* new remainder     */
            q = (q << 1) + add;                  /* q = 2·q + add     */
        }
        return q;
    }

    /* Arbitrary‑precision implementation.  seq_pow handles large power. */
    seq_t val = seq_pow((seq_t)2, exp);      /* 2^(n+5)            */
    return val / (seq_t)63;                  /* floor division     */
}
