/*
 * A230833  Sum of the first n strobogrammatic numbers.
 * https://oeis.org/A230833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of the first n strobogrammatic numbers (in increasing
 *            order)
 * Range:     u8 n <= 6, u16 n <= 31, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cache for prefix sums; sum_cache[i] holds a(i+1). */
static seq_t *sum_cache = NULL;
static size_t   sum_len   = 0;   /* number of stored terms */
static size_t   sum_cap   = 0;   /* allocated capacity */

/* Current running sum of generated numbers. */
static seq_t   running_sum = 0;

/* Next power‑of‑ten length to generate. */
static int current_len = 1;

/* Buffer for building a number digit by digit. */
static char buf[64];

/* Pair tables for strobogrammatic digits. */
static const char pairA[5] = {'0','1','6','8','9'};
static const char pairB[5] = {'0','1','9','8','6'};
static const char middle[3] = {'0','1','8'};

/* Recursive generator that fills buf[left..right] and emits full numbers. */
static void rec_gen(int left, int right, int len)
{
    if (left > right) {                /* even length finished */
        seq_t val = 0;
        for (int i = 0; i < len; ++i)
            val = val * 10 + (seq_t)(buf[i] - '0');
        running_sum += val;
        if (sum_len >= sum_cap) {
            size_t new_cap = sum_cap ? sum_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(sum_cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            sum_cache = tmp;
            sum_cap = new_cap;
        }
        sum_cache[sum_len++] = running_sum;
        return;
    }

    if (left == right) {               /* middle digit for odd length */
        for (int i = 0; i < 3; ++i) {
            buf[left] = middle[i];
            seq_t val = 0;
            for (int j = 0; j < len; ++j)
                val = val * 10 + (seq_t)(buf[j] - '0');
            running_sum += val;
            if (sum_len >= sum_cap) {
                size_t new_cap = sum_cap ? sum_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(sum_cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                sum_cache = tmp;
                sum_cap = new_cap;
            }
            sum_cache[sum_len++] = running_sum;
        }
        return;
    }

    for (int i = 0; i < 5; ++i) {
        if (left == 0 && pairA[i] == '0')
            continue;                  /* no leading zero for multi–digit numbers */
        buf[left]  = pairA[i];
        buf[right] = pairB[i];
        rec_gen(left + 1, right - 1, len);
    }
}

/* Generate all strobogrammatic numbers of a given length and update sums. */
static void generate_len(int len)
{
    rec_gen(0, len - 1, len);
}

/* Public function required by the OEIS implementation. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* behaviour below offset is irrelevant */

    while (sum_len < (size_t)n) {
        generate_len(current_len);
        ++current_len;
    }
    return sum_cache[n - 1];
}
