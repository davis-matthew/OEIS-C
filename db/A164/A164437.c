/*
 * A164437  Number of binary strings of length n with no substrings equal to
 *          0000, 0101, or 1111.
 * https://oeis.org/A164437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 41, u64 n <= 84, u128 n <= 170,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164437: number of binary strings of length n with no
 * substrings equal to 0000, 0101, or 1111.
 * Computed by a DFA with states representing the last up to 3 bits.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Recursive computation with memoisation.  seq[n] holds a(n) and
 * dp[code] holds the number of strings of the current length whose
 * last up to 3 bits encode 'code' (bits in the low 3 positions, the
 * length of the suffix in the high 2 bits).
 */
seq_t a(idx_t n)
{
    /* static cache of seq values */
    static seq_t *seq = NULL;          /* array of a(k) values */
    static size_t seq_alloc = 0;       /* allocated size */
    static idx_t computed = 0;         /* largest n computed so far */
    /* state for the automaton; 32 possible states (len 0..3, bits 0..7) */
    static seq_t dp[32] = {0};

    /* -----------------------------------------------------------------
     *  First call: initial allocation, set a(0)=1, and initialise
     *  state for length 0 (empty suffix).
     * ----------------------------------------------------------------- */
    if (seq == NULL) {
        seq_alloc = 1;
        seq = (seq_t *)calloc(seq_alloc, sizeof(seq_t));
        if (seq == NULL) return (seq_t)0;   /* allocation failure */
        seq[0] = 1;                          /* a(0) = 1 */
        dp[0] = 1;                            /* empty suffix */
        /* other dp entries are already zero from declaration */
        computed = 0;
    }

    /* -----------------------------------------------------------------
     *  Resize cache if a larger index is requested.
     * ----------------------------------------------------------------- */
    if ((size_t)n + 1 > seq_alloc) {
        size_t new_alloc = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_alloc * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        seq = tmp;
        seq_alloc = new_alloc;
    }

    /* -----------------------------------------------------------------
     *  Compute successive values up to n, starting from the last
     *  computed length.
     * ----------------------------------------------------------------- */
    if (n > computed) {
        for (idx_t len = computed + 1; len <= n; ++len) {
            seq_t newdp[32] = {0};

            for (int code = 0; code < 32; ++code) {
                seq_t val = dp[code];
                if (val == 0) continue;

                int cur_len = code >> 3;  /* length of suffix (0..3) */
                int bits    = code & 7;   /* the bits themselves */

                /* try appending 0 and 1 */
                for (int bit = 0; bit <= 1; ++bit) {
                    if (cur_len < 3) {
                        int new_len  = cur_len + 1;
                        int new_bits = (bits << 1) | bit;
                        int new_code = (new_len << 3) | new_bits;
                        newdp[new_code] += val;
                    } else { /* cur_len == 3 -> 4‑bit substring formed */
                        int sub = (bits << 1) | bit;  /* 4-bit value 0..15 */
                        if (sub == 0 || sub == 5 || sub == 15) continue; /* forbidden */
                        int new_len  = 3;
                        int new_bits = ((bits & 3) << 1) | bit;
                        int new_code = (new_len << 3) | new_bits;
                        newdp[new_code] += val;
                    }
                }
            }

            /* store counts for next iteration */
            for (int i = 0; i < 32; ++i) dp[i] = newdp[i];

            /* sum all states to get a(len) */
            seq_t total = 0;
            for (int i = 0; i < 32; ++i) total += newdp[i];
            seq[(size_t)len] = total;
        }
        computed = n;
    }

    return seq[(size_t)n];
}
