/*
 * A011575  Stirling numbers of second kind S2(26,n).
 * https://oeis.org/A011575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = Stirling number of the second kind S(26,n). For n > 26
 *            the value is 0. The recurrence used is S(n,k) =
 *            k*S(n-1,k)+S(n-1,k-1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
seq_t a(idx_t n)
{
    static seq_t table[27][27];
    static bool ready = false;

    if (!ready) {
        /* Base cases */
        for (idx_t i = 0; i <= 26; ++i)
            table[i][0] = (i == 0) ? (seq_t)1 : (seq_t)0;

        /* Stirling numbers recurrence */
        for (idx_t i = 1; i <= 26; ++i) {
            for (idx_t k = 1; k <= i; ++k) {
                table[i][k] = (seq_t)k * table[i - 1][k] + table[i - 1][k - 1];
            }
        }
        ready = true;
    }

    if (n == 0) return (seq_t)0;        /* Undefined below offset; return 0 */
    if (n <= 26) return table[26][n];   /* Requested term */
    return (seq_t)0;                    /* Terms beyond the sequence are 0 */
}
