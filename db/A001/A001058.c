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
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: sort numbers in each digit-length group by their English names in reverse alphabetical order */

#include <stdlib.h>
#include <string.h>

static void number_to_english(unsigned num, char *buf, size_t buflen) {
    static const char *ones[] = {
        "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
    };
    static const char *teens[] = {
        "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen",
        "sixteen", "seventeen", "eighteen", "nineteen"
    };
    static const char *tens[] = {
        "", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"
    };
    
    buf[0] = '\0';
    
    if (num < 10) {
        strncpy(buf, ones[num], buflen - 1);
        buf[buflen - 1] = '\0';
    } else if (num < 20) {
        strncpy(buf, teens[num - 10], buflen - 1);
        buf[buflen - 1] = '\0';
    } else if (num < 100) {
        unsigned t = num / 10;
        unsigned o = num % 10;
        strncpy(buf, tens[t], buflen - 1);
        buf[buflen - 1] = '\0';
        if (o > 0) {
            strncat(buf, "-", buflen - strlen(buf) - 1);
            strncat(buf, ones[o], buflen - strlen(buf) - 1);
            buf[buflen - 1] = '\0';
        }
    } else if (num < 1000) {
        unsigned h = num / 100;
        unsigned rest = num % 100;
        strncpy(buf, ones[h], buflen - 1);
        buf[buflen - 1] = '\0';
        strncat(buf, " hundred", buflen - strlen(buf) - 1);
        buf[buflen - 1] = '\0';
        if (rest > 0) {
            strncat(buf, " ", buflen - strlen(buf) - 1);
            buf[buflen - 1] = '\0';
            char rest_str[32];
            number_to_english(rest, rest_str, sizeof(rest_str));
            strncat(buf, rest_str, buflen - strlen(buf) - 1);
            buf[buflen - 1] = '\0';
        }
    } else {
        unsigned th = num / 1000;
        unsigned rest = num % 1000;
        strncpy(buf, ones[th], buflen - 1);
        buf[buflen - 1] = '\0';
        strncat(buf, " thousand", buflen - strlen(buf) - 1);
        buf[buflen - 1] = '\0';
        if (rest > 0) {
            strncat(buf, " ", buflen - strlen(buf) - 1);
            buf[buflen - 1] = '\0';
            char rest_str[32];
            number_to_english(rest, rest_str, sizeof(rest_str));
            strncat(buf, rest_str, buflen - strlen(buf) - 1);
            buf[buflen - 1] = '\0';
        }
    }
}

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
