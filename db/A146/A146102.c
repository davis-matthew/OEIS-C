/*
 * A146102  Bell numbers (A000110) read mod 12.
 * https://oeis.org/A146102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Bell(n) mod 12 (period 156)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* Period of the sequence is 156, so we compute the first 156 terms
       once and then answer queries by modular indexing. */
    static seq_t period[156];
    static bool ready = false;

    if (!ready) {
        /* bin[i][k] = C(i, k) mod 12 for 0 <= i <= 155 */
        seq_t bin[156][156] = {0};
        bin[0][0] = 1;
        for (idx_t i = 1; i < 156; ++i) {
            bin[i][0] = bin[i][i] = 1;
            for (idx_t k = 1; k < i; ++k) {
                bin[i][k] = (bin[i - 1][k - 1] + bin[i - 1][k]) % 12;
            }
        }

        /* Bell[i] = Bell number modulo 12 for i = 0..155 */
        seq_t Bell[156];
        Bell[0] = 1;
        for (idx_t i = 1; i < 156; ++i) {
            seq_t sum = 0;
            for (idx_t k = 0; k < i; ++k) {
                sum += (bin[i - 1][k] * Bell[k]) % 12;
                sum %= 12;
            }
            Bell[i] = sum;
        }

        for (idx_t i = 0; i < 156; ++i) {
            period[i] = Bell[i];
        }
        ready = true;
    }

    idx_t idx = n % 156;
    return period[idx];
}
