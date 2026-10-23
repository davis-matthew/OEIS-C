/*
 * A102425  Let pi be an unrestricted partition of n with the summands written
 *          as binary numbers; a(n) is the number of such partitions with an
 *          even number of binary ones.
 * https://oeis.org/A102425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unrestricted partitions of n whose summands, *
 *            written in binary, contain an even total number of 1's. * * The
 *            algorithm keeps two tables: * even[i] – #partitions of i with
 *            even popcount total * odd[i] – #partitions of i with odd
 *            popcount total * For each part size p we update th...
 * Range:     u8 n <= 19, u16 n <= 47, u32 n <= 134, u64 n <= 428,
 *            u128 n <= 1000, big any n
 * Helpers:   popcnt_parity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/popcnt_parity.h"

/* External linkage function required by the build. */
seq_t a(idx_t n)
{
    /* Allocate tables using the appropriate allocator. */
#ifdef __cplusplus
    seq_t *even = new seq_t[n + 1];
    seq_t *odd  = new seq_t[n + 1];
#else
    seq_t *even = (seq_t *)calloc(n + 1, sizeof(seq_t));
    seq_t *odd  = (seq_t *)calloc(n + 1, sizeof(seq_t));
#endif

    if (!even || !odd) {
        /* Allocation failed – return 0 as a safe failure value. */
#ifdef __cplusplus
        delete[] even; delete[] odd;
#else
        free(even); free(odd);
#endif
        return 0;
    }

    /* Initialise to zero; only even[0] = 1 for the empty partition. */
    for (idx_t i = 0; i <= n; ++i) {
        even[i] = 0;
        odd[i]  = 0;
    }
    even[0] = 1;

    /* Dynamic programming over all part sizes 1…n. */
    for (idx_t part = 1; part <= n; ++part) {
        int partParity = popcnt_parity(part);  /* 0 for even popcount, 1 for odd */
        for (idx_t sum = part; sum <= n; ++sum) {
            idx_t diff = sum - part;
            if (partParity == 0) {
                even[sum] += even[diff];
                odd[sum]  += odd[diff];
            } else {
                even[sum] += odd[diff];
                odd[sum]  += even[diff];
            }
        }
    }

    seq_t result = even[n];

    /* Clean up the allocated tables. */
#ifdef __cplusplus
    delete[] even;
    delete[] odd;
#else
    free(even);
    free(odd);
#endif

    return result;
}
