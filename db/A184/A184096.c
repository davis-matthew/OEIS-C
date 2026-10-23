/*
 * A184096  Half the number of n X n toroidal binary arrays with each element
 *          having the sum of its vertical neighbors equal to the sum of its
 *          horizontal neighbors.
 * https://oeis.org/A184096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 7*a(n-2) - 14*a(n-4) + 8*a(n-6)  for n >= 7
 * Initial values: a(1)=1, a(2)=2, a(3)=7, a(4)=18, a(5)=31, a(6)=98
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for offset 1, safe guard */
    if (n <= 6) {
        static const seq_t base[] = {0, 1, 2, 7, 18, 31, 98};
        return base[n];
    }

    seq_t *arr = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!arr) return 0;            /* allocation failure – unlikely */

    static const seq_t base[] = {0, 1, 2, 7, 18, 31, 98};
    for (idx_t i = 1; i <= 6; ++i) arr[i] = base[i];

    for (idx_t i = 7; i <= n; ++i) {
        seq_t sum = 7 * arr[i - 2] + 8 * arr[i - 6];
        arr[i] = sum - 14 * arr[i - 4];
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
