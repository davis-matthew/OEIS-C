/*
 * A164492  Number of binary strings of length n with no substrings equal to
 *          0010 0101 or 0110.
 * https://oeis.org/A164492
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Number of binary strings of length n with no substrings equal to
 *            0010, 0101, or 0110. Computation is done by a simple dynamic
 *            programming over the last up to three bits
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 42, u64 n <= 86, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*--- Required external types ------------------------------------------------*/
#include <stdint.h>   /* for any standard integer types if needed */

/*---- State description -----------------------------------------------------*/
static int state_len[15];
static int state_bits[15];
static int states_initialized = 0;

/* Map state index to (length, bits).  States 0..14 cover all suffixes
   of length 0,1,2,3 that can appear in a good string. */
static void init_states(void)
{
    int id = 0;
    state_len[id] = 0;          /* empty suffix */
    state_bits[id] = 0;
    id++;

    for (int len = 1; len <= 3; ++len) {
        for (int bits = 0; bits < (1 << len); ++bits) {
            state_len[id] = len;
            state_bits[id] = bits;
            id++;
        }
    }
}

/*--------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (!states_initialized) {
        init_states();
        states_initialized = 1;
    }

    /* current counts for each state */
    seq_t current[15] = {0};
    seq_t next[15];
    current[0] = 1;              /* empty string */

    for (idx_t pos = 0; pos < n; ++pos) {
        /* zero next array */
        for (int i = 0; i < 15; ++i) next[i] = 0;

        for (int s = 0; s < 15; ++s) {
            seq_t cnt = current[s];
            if (!cnt) continue;

            int len  = state_len[s];
            int bits = state_bits[s];

            for (int bit = 0; bit <= 1; ++bit) {
                /* Check forbidden 4‑bit pattern when len == 3 */
                if (len == 3) {
                    int pat = (bits << 1) | bit;
                    if (pat == 2 || pat == 5 || pat == 6) continue;  /* 0010, 0101, 0110 */
                }

                int newLen;
                int newBits;

                if (len < 3) {            /* still building up to 3 bits */
                    newLen  = len + 1;
                    newBits = (bits << 1) | bit;        /* exact shift, no mask needed */
                } else {                   /* len == 3, keep last two bits and new bit */
                    newLen  = 3;
                    newBits = ((bits & 3) << 1) | bit;   /* mask out the most significant bit */
                }

                int newId;
                if (newLen == 0)           newId = 0;
                else if (newLen == 1)      newId = 1 + newBits;
                else if (newLen == 2)      newId = 3 + newBits;
                else                       newId = 7 + newBits;     /* newLen == 3 */

                next[newId] += cnt;
            }
        }

        /* Move next to current for the next iteration */
        for (int i = 0; i < 15; ++i) current[i] = next[i];
    }

    /* sum of all states gives number of good strings of length n */
    seq_t result = 0;
    for (int i = 0; i < 15; ++i) result += current[i];
    return result;
}
