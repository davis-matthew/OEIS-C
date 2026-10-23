/*
 * A166695  Alternating smallest odd/even number not in list followed by that
 *          number of consecutive odd/even numbers, sequence commencing with
 *          1.
 * https://oeis.org/A166695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 248, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A166695:  Alternating smallest odd/even number not in list followed by that number of consecutive odd/even numbers,
   starting with 1.  The construction is performed by iterating blocks of increasing length:
   • an odd block begins with the smallest unused odd M and contains the M+1 consecutive odds
     M, M+2, …, M+2M;
   • the next odd block starts with M' = 3·M + 2; the same rule applies to even blocks.
   This implements the recurrence described in the OEIS entry.
*/

#include <stdbool.h>

/* seq_t and idx_t are supplied by the build environment. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For safety, if n == 0 return 0. */
    if (n == 0)
        return 0;

    idx_t pos      = 1;          /* current index of the start of the block */
    seq_t m_odd    = 1;          /* smallest unused odd number */
    seq_t m_even   = 2;          /* smallest unused even number */
    bool odd_turn  = true;       /* which parity to generate next */

    for (;;) {
        seq_t m      = odd_turn ? m_odd : m_even;
        idx_t block_len = (idx_t)(m + 1u);   /* M+1 terms in this block */

        /* Does the desired index lie in this block? */
        if (n <= pos + block_len - 1u) {
            idx_t offset = n - pos;           /* 0‑based offset within the block */
            return m + (seq_t)(2u * offset);  /* consecutive odds or evens */
        }

        /* Skip this block */
        pos += block_len;

        /* Prepare the next block of the same parity */
        if (odd_turn)
            m_odd = 3 * m_odd + 2;
        else
            m_even = 3 * m_even + 2;

        odd_turn = !odd_turn;
    }
}
