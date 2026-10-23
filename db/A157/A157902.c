/*
 * A157902  a(n) is the smallest number greater than a(n-1) whose name in UK
 *          English contains n consonants.
 * https://oeis.org/A157902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 29, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the A157902 sequence by searching for the smallest integer
   whose UK English name contains n consonants. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/seq_to_str.h"

/* Consonant counts for the English words for the numbers 1 to 19 */
static const int ones19[20] = {
    0, /*0 unused*/
    1, /*1   one      */
    2, /*2   two      */
    3, /*3   three    */
    2, /*4   four     */
    2, /*5   five     */
    2, /*6   six      */
    3, /*7   seven    */
    3, /*8   eight    */
    2, /*9   nine     */
    2, /*10  ten      */
    3, /*11  eleven   */
    4, /*12  twelve   */
    5, /*13  thirteen */
    4, /*14  fourteen */
    4, /*15  fifteen  */
    4, /*16  sixteen  */
    5, /*17  seventeen*/
    4, /*18  eighteen*/
    4  /*19  nineteen*/
};

/* Consonant counts for the tens multiples 20,30,...,90 */
static const int tens[8] = { /* tens part for 20,30,...,90 */
    4, /*20  twenty */
    4, /*30  thirty */
    3, /*40  forty  */
    3, /*50  fifty  */
    3, /*60  sixty  */
    4, /*70  seventy*/
    3, /*80  eighty */
    3  /*90  ninety */
};

/* Consonant counts for the group names */
static const int group_cons[12] = { /* consonant counts for groups */
    0, /* 0 unused */
    5, /* thousand   */
    4, /* million    */
    4, /* billion    */
    5, /* trillion   */
    6, /* quadrillion*/
    6, /* quintillion*/
    6, /* sextillion */
    6, /* septillion */
    6, /* octillion  */
    5, /* nonillion  */
    5  /* decillion  */
};

/* Count consonants for a number in [1,999] */
static int cnt_0to999(int n)
{
    if (n == 0) return 0;
    int total = 0;
    int h = n / 100;
    int rem = n % 100;
    if (h > 0) {
        total += ones19[h];  /* e.g., one, two, ... */
        total += 5;          /* "hundred" */
        if (rem > 0) {
            total += 2;      /* "and" */
        }
    }
    if (rem >= 20) {
        int t = rem / 10;
        int u = rem % 10;
        total += tens[t - 2];
        if (u > 0) total += ones19[u];
    } else if (rem > 0) {
        total += ones19[rem];
    }
    return total;
}

/* Count consonants in the UK English name of a natural number n >= 1 */
static int count_consonants(seq_t n)
{
    char buf[256];
    size_t len;
    seq_to_str(n, buf, &len);

    /* Extract groups of three decimal digits from the right (units, thousands, ...) */
    int chunks[20];
    int group_cnt = 0;
    size_t pos = len;
    while (pos > 0 && group_cnt < (int)(sizeof(chunks)/sizeof(chunks[0]))) {
        size_t start = (pos >= 3) ? (pos - 3) : 0;
        int chunk = 0;
        for (size_t i = start; i < pos; ++i) {
            chunk = chunk * 10 + (buf[i] - '0');
        }
        chunks[group_cnt++] = chunk;
        pos = start;
    }

    int total = 0;
    for (int g = 0; g < group_cnt; ++g) {
        int chunk = chunks[g];
        if (chunk == 0) continue;
        total += cnt_0to999(chunk);
        if (g > 0) {
            total += group_cons[g];
            /* Add "and" if lower part is non-zero and less than 100 */
            bool lower_other_nonzero = false;
            for (int h = 1; h < g; ++h) {
                if (chunks[h] > 0) {
                    lower_other_nonzero = true;
                    break;
                }
            }
            if (!lower_other_nonzero && chunks[0] > 0 && chunks[0] < 100) {
                total += 2;  /* "and" */
            }
        }
    }
    return total;
}

/* Static cache for computed sequence values; capacity 20000 suffices for n up to 10000 */
static seq_t cache[20000];
static idx_t cache_size = 0;

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* sequence defined for n >= 1 */

    while (cache_size < n) {
        seq_t start = (cache_size == 0) ? (seq_t)1 : cache[cache_size - 1] + (seq_t)1;
        seq_t candidate = start;
        while (true) {
            int c = count_consonants(candidate);
            if (c == (int)(cache_size + 1)) {
                cache[cache_size] = candidate;
                cache_size++;
                break;
            }
            candidate = candidate + (seq_t)1;
        }
    }
    return cache[n - 1];
}
