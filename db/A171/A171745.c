/*
 * A171745  Nonnegative integers that cannot be obtained using six or fewer
 *          4's with any combination of addition, subtraction, multiplication,
 *          or division.
 * https://oeis.org/A171745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth nonnegative integer that cannot be obtained *
 *            using six or fewer 4's with any combination of +, -, *, /. *
 *            Generation: compute all representable numbers up to 4^6 = 4096,
 *            * then all larger integers are missing
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Max value reachable with six 4's (4*4*4*4*4*4) */
#define MAX 4096

seq_t a(idx_t n) {
    /* One‑time static data */
    static bool initialized = false;
    static bool reach[7][MAX + 1];
    static bool rep[MAX + 1];
    static seq_t missing[MAX + 1];  /* at most MAX+1 entries */
    static idx_t missing_count = 0;

    if (!initialized) {
        /* Mark reachable values with exactly k fours (k = 1..6) */
        reach[1][4] = true;   /* single 4 */

        for (int k = 2; k <= 6; ++k) {
            for (int s = 1; s < k; ++s) {
                int t = k - s;
                for (int i = 0; i <= MAX; ++i) {
                    if (!reach[s][i]) continue;
                    for (int j = 0; j <= MAX; ++j) {
                        if (!reach[t][j]) continue;
                        /* addition */
                        int sum = i + j;
                        if (sum <= MAX) reach[k][sum] = true;
                        /* subtraction (non‑negative) */
                        if (i >= j) reach[k][i - j] = true;
                        /* multiplication */
                        int prod = i * j;
                        if (prod <= MAX) reach[k][prod] = true;
                        /* division (exact) */
                        if (j != 0 && i % j == 0) {
                            int quot = i / j;
                            if (quot <= MAX) reach[k][quot] = true;
                        }
                    }
                }
            }
        }

        /* Combine representable numbers with <=6 fours */
        for (int i = 0; i <= MAX; ++i) {
            bool any = false;
            for (int k = 1; k <= 6; ++k) {
                if (reach[k][i]) { any = true; break; }
            }
            rep[i] = any;
        }

        /* Build list of missing numbers up to MAX */
        for (int i = 0; i <= MAX; ++i) {
            if (!rep[i]) missing[missing_count++] = (seq_t)i;
        }

        initialized = true;
    }

    if (n <= missing_count) {
        return missing[(size_t)n - 1];
    } else {
        seq_t offset = (seq_t)MAX;
        seq_t idx    = (seq_t)(n - missing_count);
        return offset + idx;
    }
}
