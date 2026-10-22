/*
 * A003679  Numbers that are not the sum of 3 pentagonal numbers.
 * https://oeis.org/A003679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   sieve. Mark every sum P(i)+P(j)+P(k) of three pentagonal numbers
 *            P(m) = m*(3*m-1)/2, with m >= 0 so P(0) = 0 counts, up to 33100;
 *            a(n) is the n-th positive integer that is not marked
 * Range:     u8 n <= 61, u16 n <= 210, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    static seq_t *sequence = NULL;
    static idx_t seq_len = 0;

    if (sequence == NULL) {
        const long long LIMIT = 33100LL;
        long long pent[400];
        int np = 0;
        bool *can_sum = (bool *)malloc((size_t)(LIMIT + 1) * sizeof(bool));
        memset(can_sum, 0, (size_t)(LIMIT + 1) * sizeof(bool));

        for (long long k = 0; ; k++) {
            long long p = k * (3 * k - 1) / 2;
            if (p > LIMIT) break;
            pent[np++] = p;
        }

        for (int i = 0; i < np; i++) {
            for (int j = i; j < np; j++) {
                long long s2 = pent[i] + pent[j];
                if (s2 > LIMIT) break;
                for (int k = j; k < np; k++) {
                    long long s = s2 + pent[k];
                    if (s > LIMIT) break;
                    can_sum[s] = true;
                }
            }
        }

        sequence = (seq_t *)malloc(400 * sizeof(seq_t));
        seq_len = 0;
        for (long long i = 1; i <= LIMIT && seq_len < 400; i++) {
            if (!can_sum[i]) {
                sequence[(size_t)seq_len] = (seq_t)i;
                seq_len++;
            }
        }
        free(can_sum);
    }

    if (n >= 1 && n <= seq_len) {
        return sequence[(size_t)(n - 1)];
    }
    return 0;
}
