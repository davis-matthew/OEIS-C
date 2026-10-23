/*
 * A131834  Indices of records in A100949.
 * https://oeis.org/A131834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A131834
   Computes the indices of records in A100949.
   For each n, counts the partitions n = p + s where p is a prime
   and s is a semiprime (product of two primes, possibly equal).
   Records n whenever that count is greater than all previous counts.
   a(n) returns the n‑th such record index. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Static state                                                          */
/* --------------------------------------------------------------------- */
static idx_t   seq_limit      = 0;      /* maximal n processed so far       */
static idx_t * prime_list     = NULL;   /* list of primes up to seq_limit   */
static size_t   prime_cnt      = 0;      /* number of primes                 */
static unsigned char *semiprime_arr = NULL; /* semiprime flags 0/1             */
static seq_t *  count_arr      = NULL;   /* partition counts for each n      */
static idx_t *  record_arr     = NULL;   /* record indices                   */
static size_t   record_cnt     = 0;      /* number of records found          */

/* --------------------------------------------------------------------- */
/* Helper: build prime list up to limit                                 */
/* --------------------------------------------------------------------- */
static void build_primes(idx_t limit)
{
    if (prime_list) { free(prime_list); prime_list = NULL; }
    bool *is_composite = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_composite) exit(1);

    for (idx_t i = 2; i <= limit / i; ++i)
        if (!is_composite[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;

    prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) ++prime_cnt;

    prime_list = (idx_t*)malloc(prime_cnt * sizeof(idx_t));
    if (!prime_list) exit(1);
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) prime_list[pos++] = i;

    free(is_composite);
}

/* --------------------------------------------------------------------- */
/* Helper: build semiprime flag array up to limit                        */
/* --------------------------------------------------------------------- */
static void build_semiprimes(idx_t limit)
{
    if (semiprime_arr) { free(semiprime_arr); semiprime_arr = NULL; }
    semiprime_arr = (unsigned char*)calloc((size_t)(limit + 1), sizeof(unsigned char));
    if (!semiprime_arr) exit(1);

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        for (size_t j = i; j < prime_cnt; ++j) {
            idx_t q = prime_list[j];
            uint64_t prod = (uint64_t)p * (uint64_t)q;
            if (prod > limit) break;
            semiprime_arr[(size_t)prod] = 1;
        }
    }
}

/* --------------------------------------------------------------------- */
/* Rebuild all data structures up to new_limit                           */
/* --------------------------------------------------------------------- */
static void rebuild(idx_t new_limit)
{
    /* Allocate counts array */
    if (count_arr) { free(count_arr); count_arr = NULL; }
    count_arr = (seq_t*)malloc((size_t)(new_limit + 1) * sizeof(seq_t));
    if (!count_arr) exit(1);

    /* Rebuild primes and semiprimes for new_limit */
    build_primes(new_limit);
    build_semiprimes(new_limit);

    /* Reset record array */
    if (record_arr) { free(record_arr); record_arr = NULL; }
    record_arr = NULL;
    record_cnt = 0;

    seq_t max_seen = 0;
    for (idx_t n = 1; n <= new_limit; ++n) {
        seq_t c = 0;
        for (size_t k = 0; k < prime_cnt && prime_list[k] <= n; ++k) {
            idx_t p = prime_list[k];
            if (semiprime_arr[(size_t)(n - p)]) ++c;
        }
        count_arr[n] = c;
        if (c > max_seen) {
            max_seen = c;
            /* grow record array on demand */
            if (record_cnt == 0) {
                record_arr = (idx_t*)malloc(4 * sizeof(idx_t));
                if (!record_arr) exit(1);
            } else if (record_cnt >= 4 * (record_cnt / 4)) {
                idx_t *tmp = (idx_t*)realloc(record_arr, (record_cnt + 4) * sizeof(idx_t));
                if (!tmp) exit(1);
                record_arr = tmp;
            }
            record_arr[record_cnt++] = n;
        }
    }
    seq_limit = new_limit;
}

/* --------------------------------------------------------------------- */
/* Ensure we have at least 'need' records                                 */
/* --------------------------------------------------------------------- */
static void ensure_records(idx_t need)
{
    while (record_cnt < need) {
        idx_t new_limit = seq_limit == 0 ? 20000ULL : seq_limit * 3 / 2 + 100ULL;
        if (new_limit <= seq_limit) new_limit = seq_limit + 1000ULL;
        rebuild(new_limit);
    }
}

/* --------------------------------------------------------------------- */
/* The required function                                                 */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */
    ensure_records(n);             /* build enough records */
    return (seq_t)record_arr[n - 1];
}
