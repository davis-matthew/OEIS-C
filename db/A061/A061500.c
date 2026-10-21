/*
 * A061500  Integer part of arithmetic mean of distinct nonzero numbers that
 *          can be formed from the digits of n.
 * https://oeis.org/A061500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor( (sum of distinct non‑zero numbers formed from the
 *            digits of n) / (number of such distinct numbers) )
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   cmp_ull, gen_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/cmp_ull.h"
#include "../utils/gen_rec.h"

/* Comparator for sorting 8‑bit digit arrays */
static int cmp_u8(const void *a, const void *b)
{
    return (*(uint8_t *)a) - (*(uint8_t *)b);
}

seq_t a(idx_t n)
{
    /* Extract decimal digits of n (least‑significant first) */
    uint8_t digits[20];
    int d = 0;
    idx_t tmp = n;
    do {
        digits[d++] = tmp % 10;
        tmp /= 10;
    } while (tmp > 0);

    /* Sort digits to enable duplicate skipping during generation */
    qsort(digits, d, sizeof(uint8_t), cmp_u8);

    /* Storage for all generated numbers (duplicates allowed) */
    const size_t MAX_NUMS = 4000;   /* sufficient for n up to 10 000 */
    unsigned long long arr[MAX_NUMS];
    size_t cnt = 0;
    bool used[20] = {false};

    gen_rec(0, 0ULL, digits, d, used, &cnt, arr);

    if (cnt == 0) return 0;        /* n >= 1 guarantees this never happens */

    /* Sort to facilitate deduplication */
    qsort(arr, cnt, sizeof(unsigned long long), cmp_ull);

    /* Compute sum and number of distinct non‑zero numbers */
    unsigned long long sum = 0;
    size_t distinct = 0;
    unsigned long long last = 0;
    bool first = true;

    for (size_t i = 0; i < cnt; ++i) {
        if (first || arr[i] != last) {
            sum += arr[i];
            last = arr[i];
            ++distinct;
            first = false;
        }
    }

    /* Integer part of the arithmetic mean */
    seq_t result = (seq_t)(sum / distinct);
    return result;
}
