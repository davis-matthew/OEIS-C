/*
 * A061589  a(1) = 11, a(n) = number obtained by adding to each digit of
 *          a(n-1) the next digit.
 * https://oeis.org/A061589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = concatenate all digit‑wise sums of adjacent digits of
 *            a(n‑1) (written as decimal strings), followed by the last digit
 *            of a(n‑1); a(1) = 11. This recurrence produces A061589
 * Range:     u8 n <= 12, u16 n <= 16, u32 n <= 20, u64 n <= 23, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
        /* Static cache: seq of computed values and the current string.  */
        static seq_t *values = NULL;           /* values[0] = a(1), ...   */
        static size_t cache_len = 0;           /* number of values stored */
        static size_t cache_cap = 0;           /* allocated capacity       */
        static char *last_str = NULL;          /* decimal digits of a(n)   */
        static size_t last_len = 0;            /* length of last_str       */

        if (n == 0) return 0;   /* undefined for offset 1, but harmless */

        while (cache_len < n) {
                if (cache_len == 0) {          /* initialise a(1) = 11   */
                        /* store the string "11" */
                        last_len = 2;
                        last_str = (char*)malloc(3);   /* 2 digits + '\0' */
                        last_str[0] = '1';
                        last_str[1] = '1';
                        last_str[2] = '\0';

                        /* store the numeric value 11 */
                        if (cache_cap == 0) {
                                cache_cap = 1;
                                values = (seq_t*)malloc(cache_cap * sizeof(seq_t));
                        }
                        values[cache_len++] = 11;
                } else {                       /* compute next term       */
                        const char *prev = last_str;
                        size_t prev_len = last_len;

                        /* estimate maximum new length: each sum ≤ 18 (2 digits) */
                        size_t new_cap = prev_len * 2 + 1;
                        char *new_str = (char*)malloc(new_cap + 1);
                        size_t idx = 0;

                        for (size_t i = 0; i < prev_len - 1; ++i) {
                                int s = (prev[i] - '0') + (prev[i + 1] - '0');
                                if (s >= 10) {
                                        new_str[idx++] = '0' + s / 10;
                                        new_str[idx++] = '0' + s % 10;
                                } else {
                                        new_str[idx++] = '0' + s;
                                }
                        }
                        /* append the last digit of the previous term */
                        new_str[idx++] = prev[prev_len - 1];
                        new_str[idx] = '\0';

                        /* convert the new string to seq_t */
                        seq_t val = 0;
                        for (size_t j = 0; j < idx; ++j) {
                                val = val * 10 + (new_str[j] - '0');
                        }

                        /* grow the values array if needed */
                        if (cache_len == cache_cap) {
                                size_t new_cap_vals = (cache_cap == 0) ? 1 : cache_cap * 2;
                                seq_t *tmp = (seq_t*)realloc(values, new_cap_vals * sizeof(seq_t));
                                values = tmp;
                                cache_cap = new_cap_vals;
                        }
                        values[cache_len++] = val;

                        /* recycle the previous string */
                        free(last_str);
                        last_str = new_str;
                        last_len = idx;
                }
        }

        return values[n - 1];
}
