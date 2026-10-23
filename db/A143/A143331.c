/*
 * A143331  Lengths of successive runs of 0's in the Thue-Morse sequence
 *          A010060.
 * https://oeis.org/A143331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A026465(2n-1) – run lengths of successive 0's in the
 *            Thue–Morse sequence, computed by iterating the parity of the
 *            binary representation of indices
 * Range:     u8 n <= 9600, u16 n <= 9600, u32 n <= 9600, u64 n <= 9600,
 *            u128 n <= 9600, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static inline bool thue_morse_bit(idx_t i)
{
    /* parity of the number of 1-bits in i (0 → 0, 1 → 1, etc.) */
    return __builtin_parityll(i);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not needed for OEIS offset 1, but harmless */

    idx_t bit_index = 0;           /* current index in the Thue–Morse sequence */
    bool  current_bit = 0;         /* value of the previous bit (initialized on first iteration) */
    seq_t run_len     = 0;         /* length of the current run of identical bits */
    idx_t zero_runs   = 0;         /* number of zero runs completed so far */

    while (1)
    {
        bool bit = thue_morse_bit(bit_index);

        if (bit_index == 0)            /* start of the sequence */
        {
            current_bit = bit;
            run_len = 1;
        }
        else if (bit == current_bit)   /* still within the same run */
        {
            run_len++;
        }
        else                           /* run finished (bit changed) */
        {
            if (current_bit == 0)      /* we just finished a run of zeros */
            {
                zero_runs++;
                if (zero_runs == n)   /* found the n‑th zero run */
                    return run_len;
            }
            current_bit = bit;
            run_len = 1;
        }

        bit_index++;                   /* advance to next position */
    }
}
