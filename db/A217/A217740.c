/*
 * A217740  Abundant numbers with abundant subscripts.
 * https://oeis.org/A217740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 4030, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A217740: Meta‑abundant numbers
   Formula: a(n) = A005101( A005101(n) )
   where A005101 is the sequence of abundant numbers (offset 1).  */

/* <stdlib.h> is needed for malloc / realloc / free.
   The build supplies the types seq_t and idx_t. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* A pre‑computation limit that comfortably covers n up to ~10 000.
   With this limit there are well over 2 000 000 integers, comfortably
   exceeding the 40 000‑plus abundant numbers needed for the 10 000th
   abundant index. */
static const idx_t LIMIT = 2000000;

/* Array of abundant numbers, 0‑based.  abund_nums[i] = A005101(i+1). */
static seq_t *abund_nums = NULL;
static size_t abund_count = 0;

/* Compute all abundant numbers up to LIMIT once. */
static void precompute_abundant(void)
{
    if (abund_nums != NULL)
        return;          /* already done */

    /* Sum of proper divisors for each integer up to LIMIT. */
    uint64_t *sum = (uint64_t *)malloc((LIMIT + 1) * sizeof(uint64_t));
    if (!sum) exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= LIMIT; ++i)
        sum[i] = 0;

    for (idx_t i = 1; i <= LIMIT; ++i)
        for (idx_t j = i << 1; j <= LIMIT; j += i)
            sum[j] += i;

    /* Allocate an initial estimate (~0.3 * LIMIT). */
    size_t cap = LIMIT / 3 + 16;
    abund_nums = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!abund_nums) {
        free(sum);
        exit(EXIT_FAILURE);
    }

    /* Gather abundant numbers. */
    for (idx_t i = 1; i <= LIMIT; ++i) {
        if (sum[i] > i) {           /* i is abundant */
            if (abund_count == cap) {
                cap <<= 1;          /* double the capacity */
                seq_t *tmp = (seq_t *)realloc(abund_nums, cap * sizeof(seq_t));
                if (!tmp) {
                    free(sum);
                    exit(EXIT_FAILURE);
                }
                abund_nums = tmp;
            }
            abund_nums[abund_count++] = (seq_t)i;
        }
    }
    free(sum);                      /* no longer needed */
}

/* External linkage function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* out of range, harmless */

    precompute_abundant();

    if ((idx_t)n > abund_count)
        return 0;                   /* n is too large for our precomputation */

    /* Step 1: find the n‑th abundant number. */
    seq_t nth_abund = abund_nums[n - 1];   /* A005101(n) */

    /* Step 2: treat that value as an index into the abundant list. */
    idx_t idx = (idx_t) nth_abund;     /* safe: nth_abund <= LIMIT */
    if (idx == 0 || idx > abund_count)
        return 0;                   /* safety check, should not happen */

    /* Step 3: return the abundant number at that index. */
    return abund_nums[idx - 1];       /* A005101(A005101(n)) */
}
