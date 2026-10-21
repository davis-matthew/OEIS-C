/*
 * A090499  Group the natural numbers so that the sum of the numbers of
 *          divisors of the members of the (n+1)th group exceeds that for the
 *          n-th group. Sequence contains the first term of each group.
 * https://oeis.org/A090499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This function builds the groups of natural numbers required by
 *            OEIS sequence A090499. The sequence a(n) is the first member of
 *            the nth group, where each successive group has a sum of
 *            divisor‑counts that is strictly larger than the preceding group.
 *            The algorithm incrementally adds numbers to the n...
 * Range:     u8 n <= 33, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include standard headers */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* helper to generate primes up to a given limit */
#define MAX_PRIME_LIST_SIZE 2000000
static uint32_t prime_list[MAX_PRIME_LIST_SIZE];
static size_t prime_cnt = 0;
static int primes_initialized = 0;

static void init_primes(void) {
    if (primes_initialized) return;
    primes_initialized = 1;
    size_t limit = MAX_PRIME_LIST_SIZE;
    uint8_t *sieve = (uint8_t*)calloc(limit + 1, 1);
    size_t i, j;
    for (i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            if (prime_cnt < MAX_PRIME_LIST_SIZE)
                prime_list[prime_cnt++] = (uint32_t)i;
            if (i * i <= limit) {
                for (j = i * i; j <= limit; j += i)
                    sieve[j] = 1;
            }
        }
    }
    free(sieve);
}

/* divisor count for unsigned 64‑bit integer */
static seq_t divisor_count_uint64(uint64_t x) {
    init_primes();
    seq_t res = 1;
    size_t i;
    for (i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > x)
            break;
        if (x % p == 0) {
            uint32_t exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            res *= (seq_t)(exp + 1);
        }
    }
    if (x > 1)  /* remaining prime factor */
        res *= 2;
    return res;
}

/* main sequence function */
seq_t a(idx_t n) {
    static seq_t *group_start = NULL;  /* array of first element per group */
    static size_t group_allocated = 0;
    static size_t group_count = 0;     /* number of groups already computed */
    static idx_t next_num = 1;         /* next natural number to process */
    static seq_t prev_sum = 0;         /* sum of divisor counts of previous group */

    /* initial allocation */
    if (group_start == NULL) {
        group_allocated = 16;
        group_start = (seq_t*)malloc(group_allocated * sizeof(seq_t));
        if (!group_start)
            return 0; /* allocation error – return 0 as safe value */
    }

    /* compute groups until we have at least n groups */
    while (group_count < n) {
        /* start new group */
        idx_t start = next_num;
        seq_t group_sum = 0;

        while (group_sum <= prev_sum) {
            group_sum += divisor_count_uint64(next_num);
            ++next_num;
        }

        /* store start of this group */
        if (group_count == group_allocated) {
            group_allocated *= 2;
            seq_t *tmp = (seq_t*)realloc(group_start, group_allocated * sizeof(seq_t));
            if (!tmp) return 0;
            group_start = tmp;
        }
        group_start[group_count++] = (seq_t)start;
        prev_sum = group_sum;
    }

    return group_start[n - 1];
}
