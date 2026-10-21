/*
 * A213929  Number of ways of obtaining a weight of n grams with a two-pan
 *          balance using eight weights of denominations 1, 1, 2, 5, 10, 10,
 *          20 and 50 grams.
 * https://oeis.org/A213929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to weigh n grams on a two‑pan balance
 *            using eight weights of denominations 1, 1, 2, 5, 10, 10, 20, 50
 *            grams. Each weight can be placed on the left pan (−1), on the
 *            right pan (+1) or not used (0). Count the assignments whose net
 *            weight equals n
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>


static const int WEIGHTS[8] = {1, 1, 2, 5, 10, 10, 20, 50};
#define TABLE_SIZE 100

static seq_t TABLE[TABLE_SIZE];
static bool PRECOMPUTED = false;

static void compute_table(void)
{
    size_t i;
    for (i = 0; i < TABLE_SIZE; ++i)
        TABLE[i] = 0;

    /* 3^8 = 6561 possible assignments */
    int total = 1;
    for (int j = 0; j < 8; ++j)
        total *= 3;

    for (int code = 0; code < total; ++code) {
        int tmp = code;
        int sum = 0;
        for (int k = 0; k < 8; ++k) {
            int digit = tmp % 3;          /* 0,1,2 */
            tmp /= 3;
            int coeff = digit - 1;        /* -1,0,+1 */
            sum += coeff * WEIGHTS[k];
        }
        if (sum >= 0 && sum < TABLE_SIZE)
            TABLE[(size_t)sum] += 1;
    }
    PRECOMPUTED = true;
}

seq_t a(idx_t n)
{
    if (n >= TABLE_SIZE)
        return 0;
    if (!PRECOMPUTED)
        compute_table();
    return TABLE[(size_t)n];
}
