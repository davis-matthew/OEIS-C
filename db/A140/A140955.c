/*
 * A140955  Even integers that do not have at least two partitions into 2
 *          distinct primes.
 * https://oeis.org/A140955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A140955: Even integers that do NOT have at least two partitions
   into two distinct primes.  The terms start with 0, 2, 4, 6, 8, 10, 12, 14, 38, ... */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Utility for growing buffers (compatible with C++ and C) ---------- */
static void *grow_buffer(void *ptr, size_t *cap, size_t needed, size_t elem_size)
{
    if (*cap >= needed) return ptr;
    size_t new_cap = (*cap) ? (*cap) * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    void *p = realloc(ptr, new_cap * elem_size);
    *cap = new_cap;
    return p;
}

/* ---------- Data needed for the sequence ---------- */
static seq_t *seq_vals = NULL;          /* cached sequence values */
static size_t seq_cap = 0;              /* capacity of seq_vals */
static size_t seq_len = 0;              /* number of values computed */

static uint64_t next_even = 0;          /* next even number to test (0,2,4,...) */

static uint8_t *is_prime = NULL;        /* prime sieve array (1 == prime) */
static size_t prime_sieve_cap = 0;      /* current capacity of the sieve array */

static uint32_t *prime_list = NULL;     /* ascending list of primes */
static size_t prime_cap = 0;            /* capacity of prime_list */
static size_t prime_cnt = 0;            /* number of primes stored */

/* ---------- Prime sieve utilities ---------- */
static void rebuild_prime_list(void)
{
    /* free old list */
    free(prime_list);
    prime_list = NULL;
    prime_cnt = 0;
    prime_cap = 0;

    /* initial reservation: at most half of numbers up to current sieve cap */
    size_t reserve = prime_sieve_cap / 2 + 10;
    prime_list = (uint32_t *)malloc(reserve * sizeof(uint32_t));
    prime_cap  = reserve;

    for (size_t i = 2; i <= prime_sieve_cap; ++i) {
        if (is_prime[i]) {
            if (prime_cnt >= prime_cap) {
                prime_cap *= 2;
                prime_list = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
            }
            prime_list[prime_cnt++] = (uint32_t)i;
        }
    }
}

static void ensure_sieve(size_t limit)
{
    if (prime_sieve_cap > limit) return;

    /* allocate new filled sieve */
    uint8_t *new_arr = (uint8_t *)malloc(limit + 1);
    for (size_t i = 0; i <= limit; ++i) new_arr[i] = 1;
    new_arr[0] = new_arr[1] = 0;

    /* simple Sieve of Eratosthenes */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (new_arr[i]) {
            for (size_t j = i * i; j <= limit; j += i) new_arr[j] = 0;
        }
    }

    /* replace old sieve */
    free(is_prime);
    is_prime = new_arr;
    prime_sieve_cap = limit;

    /* rebuild list of primes */
    rebuild_prime_list();
}

/* ---------- Count unordered distinct prime pairs summing to an even number ---------- */
static int count_distinct_prime_pairs(uint64_t even)
{
    if (even < 4) return 0;                         /* 0 and 2 have no partitions */
    ensure_sieve((size_t)even);

    int cnt = 0;
    size_t half = even / 2;

    for (size_t i = 0; i < prime_cnt && prime_list[i] <= half; ++i) {
        uint32_t p = prime_list[i];
        uint64_t q = even - p;
        if (q <= p) break;                         /* avoid double counting, distinct required */

        if (q <= (uint64_t)prime_sieve_cap && is_prime[(size_t)q]) {
            ++cnt;
            if (cnt >= 2) break;                   /* we only care about at least two */
        }
    }
    return cnt;
}

/* ---------- Append a new term to the cached sequence ---------- */
static void append_value(seq_t val)
{
    seq_vals = (seq_t *)grow_buffer(seq_vals, &seq_cap, seq_len + 1, sizeof(seq_t));
    seq_vals[seq_len++] = val;
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1; below offset is undefined */

    while (seq_len < (size_t)n) {
        uint64_t ev = next_even;   /* current even to test */
        next_even += 2;            /* next candidate */

        if (count_distinct_prime_pairs(ev) < 2) {
            append_value((seq_t)ev);
        }
    }
    return seq_vals[n - 1];
}
