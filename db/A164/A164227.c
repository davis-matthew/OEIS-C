/*
 * A164227  Number of binary strings of length n with equal numbers of 00010
 *          and 11100 substrings.
 * https://oeis.org/A164227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary strings of length n with equal numbers
 *            of substrings 00010 and 11100
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /*  states are all prefixes of the two patterns:
        "", "0", "00", "000", "0001", "1", "11", "111", "1110"   */
    struct State { int len; unsigned int bits; };
    const struct State states[9] = {
        {0,0},            /* ""       */
        {1,0},            /* "0"      */
        {2,0},            /* "00"     */
        {3,0},            /* "000"    */
        {4,1},            /* "0001"   */
        {1,1},            /* "1"      */
        {2,3},            /* "11"     */
        {3,7},            /* "111"    */
        {4,14}            /* "1110"   */
    };
    const int state_count = 9;
    const int PAT1_BITS = 2;   /* binary 00010 */
    const int PAT2_BITS = 28;  /* binary 11100 */

    /*  Precompute transitions and differences */
    int trans_state[9][2];
    int trans_inc[9][2];
    for (int s = 0; s < state_count; ++s) {
        for (int b = 0; b < 2; ++b) {
            int t_len  = states[s].len + 1;
            int t_bits = (states[s].bits << 1) | b;
            int inc = 0;
            if (t_len == 5) {
                if (t_bits == PAT1_BITS) inc = 1;
                else if (t_bits == PAT2_BITS) inc = -1;
            }
            /* Find longest suffix that is a prefix of any pattern */
            int maxL = t_len < 4 ? t_len : 4;
            int ns = 0;   /* default fallback */
            for (int L = maxL; L >= 0; --L) {
                int suffix = t_bits & ((1 << L) - 1);
                /* search for state with len==L and bits==suffix */
                for (int t = 0; t < state_count; ++t) {
                    if (states[t].len == L && states[t].bits == (unsigned int)suffix) {
                        ns = t;
                        goto found;
                    }
                }
            }
found:
            trans_state[s][b] = ns;
            trans_inc[s][b]   = inc;
        }
    }

    /*  Dynamic programming setup  */
    /*  maximum possible difference in counts for length n  */
    int max_diff = (n >= 5) ? ((int)n - 1) / 4 : 0;
    int size    = 2 * max_diff + 1;
    int offset  = max_diff;
    size_t total = (size_t)state_count * (size_t)size;

#if defined(__cplusplus)
    seq_t *dp_cur  = new seq_t[total];
    seq_t *dp_next = new seq_t[total];
#else
    seq_t *dp_cur  = (seq_t*)malloc(total * sizeof(seq_t));
    seq_t *dp_next = (seq_t*)malloc(total * sizeof(seq_t));
#endif

    /*  initialize arrays to zero  */
    for (size_t i = 0; i < total; ++i) {
        dp_cur[i]  = (seq_t)0;
        dp_next[i] = (seq_t)0;
    }
    /*  initial state: empty string, difference 0  */
    dp_cur[offset] = (seq_t)1;

    /*  iterate over string positions  */
    for (idx_t step = 1; step <= n; ++step) {
        int cur_max = (step >= 5) ? ((int)step - 1) / 4 : 0;
        /*  clear next table  */
        for (size_t i = 0; i < total; ++i) dp_next[i] = (seq_t)0;

        for (int s = 0; s < state_count; ++s) {
            for (int d = -cur_max; d <= cur_max; ++d) {
                seq_t val = dp_cur[s * size + (d + offset)];
                if (val == (seq_t)0) continue;
                for (int b = 0; b < 2; ++b) {
                    int ns = trans_state[s][b];
                    int nd = d + trans_inc[s][b];
                    dp_next[ns * size + (nd + offset)] += val;
                }
            }
        }
        /*  swap tables  */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /*  sum all states with difference 0  */
    seq_t result = (seq_t)0;
    for (int s = 0; s < state_count; ++s) {
        result += dp_cur[s * size + offset];
    }

#if defined(__cplusplus)
    delete[] dp_cur;
    delete[] dp_next;
#else
    free(dp_cur);
    free(dp_next);
#endif

    return result;
}
