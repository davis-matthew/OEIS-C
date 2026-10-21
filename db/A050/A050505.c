/*
 * A050505  Unlucky numbers.
 * https://oeis.org/A050505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 207, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_lucky_numbers, is_lucky_in_list
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth positive integer that is NOT a lucky number
   (the complement of A000959). This implementation builds the lucky
   numbers up to a limit and then returns the nth missing number. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_lucky_numbers.h"
#include "../utils/is_lucky_in_list.h"

static bool initialized = false;
static int *lucky_arr = NULL;
static int lucky_len = 0;
static uint32_t *non_lucky_arr = NULL;
static size_t non_lucky_len = 0;
static int current_limit = 0;

#define INIT_LIMIT 200000   /* sufficient for n up to 10 000 */

static void rebuild_sequence(int limit)
{
    free(lucky_arr);
    free(non_lucky_arr);

    build_lucky_numbers(limit, &lucky_arr, &lucky_len);
    current_limit = limit;

    size_t cap = limit / 2 + 100;
    non_lucky_arr = (uint32_t*)malloc(cap * sizeof(uint32_t));
    non_lucky_len = 0;

    for (int val = 2; val <= limit; ++val) {
        if (!is_lucky_in_list(val, lucky_arr, lucky_len)) {
            if (non_lucky_len == cap) {
                cap <<= 1;
                non_lucky_arr = (uint32_t*)realloc(non_lucky_arr, cap * sizeof(uint32_t));
            }
            non_lucky_arr[non_lucky_len++] = (uint32_t)val;
        }
    }
    initialized = true;
}

static void init_sequence(void)
{
    if (initialized) return;
    if (current_limit == 0) current_limit = INIT_LIMIT;
    rebuild_sequence(current_limit);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; undef for 0 but avoid UB */
    init_sequence();

    if (n - 1 < non_lucky_len)
        return (seq_t)non_lucky_arr[n - 1];

    /* fallback: extend the limit until the nth non‑lucky is found */
    int limit = current_limit;
    while (true) {
        limit <<= 1;
        rebuild_sequence(limit);
        if (n - 1 < non_lucky_len) {
            return (seq_t)non_lucky_arr[n - 1];
        }
    }
}
