/*
 * A001058  1-digit numbers in reverse alphabetical order, then 2-digit
 *          numbers, etc.
 * https://oeis.org/A001058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   number_to_english
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: sort numbers in each digit-length group by their English names in reverse alphabetical order */

#include <stdlib.h>
#include <string.h>
#include "../utils/number_to_english.h"

typedef struct {
    unsigned num;
    char name[64];
} NumberName;

static int compare_reverse(const void *a, const void *b) {
    const NumberName *aa = (const NumberName *)a;
    const NumberName *bb = (const NumberName *)b;
    return strcmp(bb->name, aa->name);
}

seq_t a(idx_t n) {
    if (n <= 10) {
        unsigned offset = (unsigned)(n - 1);
        NumberName nums[10];
        for (int i = 0; i < 10; i++) {
            nums[i].num = i;
            number_to_english(i, nums[i].name, sizeof(nums[i].name));
        }
        qsort(nums, 10, sizeof(NumberName), compare_reverse);
        return (seq_t)nums[offset].num;
    } else if (n <= 100) {
        unsigned offset = (unsigned)(n - 11);
        NumberName nums[90];
        for (int i = 0; i < 90; i++) {
            nums[i].num = 10 + i;
            number_to_english(10 + i, nums[i].name, sizeof(nums[i].name));
        }
        qsort(nums, 90, sizeof(NumberName), compare_reverse);
        return (seq_t)nums[offset].num;
    } else if (n <= 1000) {
        unsigned offset = (unsigned)(n - 101);
        NumberName nums[900];
        for (int i = 0; i < 900; i++) {
            nums[i].num = 100 + i;
            number_to_english(100 + i, nums[i].name, sizeof(nums[i].name));
        }
        qsort(nums, 900, sizeof(NumberName), compare_reverse);
        return (seq_t)nums[offset].num;
    } else {
        unsigned offset = (unsigned)(n - 1001);
        NumberName *nums = (NumberName *)malloc(9000 * sizeof(NumberName));
        for (int i = 0; i < 9000; i++) {
            nums[i].num = 1000 + i;
            number_to_english(1000 + i, nums[i].name, sizeof(nums[i].name));
        }
        qsort(nums, 9000, sizeof(NumberName), compare_reverse);
        seq_t result = (seq_t)nums[offset].num;
        free(nums);
        return result;
    }
}
