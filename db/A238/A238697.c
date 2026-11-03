/*
 * A238697  Quotients connected with the Banach matchboxes problem:
 *          Sum_{i=1..prime(n)-7} 2^(i-1)*binomial(i+2,3)/prime(n) (case 3).
 * https://oeis.org/A238697
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..prime(n)-7} 2^(i-1)*binomial(i+2,3)/prime(n)
 *            (case 3)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 21 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

static void add_power_mul(uint64_t *sum, int *len_sum,
                          const uint64_t *power, int len_power,
                          uint64_t binom)
{
    int max_len = (*len_sum > len_power) ? *len_sum : len_power;
    if (max_len > *len_sum) {
        for (int i = *len_sum; i < max_len; ++i) sum[i] = 0;
        *len_sum = max_len;
    }

    __uint128_t carry = 0;
    int i;
    for (i = 0; i < len_power; ++i) {
        __uint128_t prod = (__uint128_t)power[i] * binom + sum[i] + carry;
        sum[i]   = (uint64_t)prod;
        carry    = prod >> 64;
    }
    while (carry) {
        if (i < *len_sum) {
            __uint128_t prod = (__uint128_t)sum[i] + carry;
            sum[i]   = (uint64_t)prod;
            carry    = prod >> 64;
        } else {
            sum[i]   = (uint64_t)carry;
            *len_sum = i + 1;
            carry    = 0;
        }
        ++i;
    }
}

static void div_by_small(uint64_t *a, int *len, uint64_t div)
{
    uint64_t carry = 0;
    for (int i = *len - 1; i >= 0; --i) {
        __uint128_t cur = ((__uint128_t)carry << 64) | a[i];
        a[i] = (uint64_t)(cur / div);
        carry = (uint64_t)(cur % div);
    }
    while (*len > 0 && a[*len - 1] == 0) --(*len);
}

static void mul_small_inplace(uint64_t *a, int *len, uint64_t m)
{
    __uint128_t carry = 0;
    for (int i = 0; i < *len; ++i) {
        __uint128_t prod = (__uint128_t)a[i] * m + carry;
        a[i]   = (uint64_t)prod;
        carry  = prod >> 64;
    }
    if (carry) {
        a[*len] = (uint64_t)carry;
        ++(*len);
    }
}

/*
 * Big‑integer helpers
 * The representation is little endian base 2^64 (least significant word first).
 * All arithmetic fits into 128‑bit temporaries.
 */

#define MAX_WORDS 2000   /* enough for prime(10000) ≈ 104729 → 2^104722 bits */

seq_t a(idx_t n)
{
    uint64_t prime = get_prime(n);

    /* For n=4 (prime 7) the sum is empty → 0 */
    if (prime <= 7) return (seq_t)0;

    idx_t M = (idx_t)prime - 7;          /* upper limit of the sum (inclusive) */

    static uint64_t power[MAX_WORDS];
    static uint64_t sum_arr[MAX_WORDS];
    int len_power = 1;
    int len_sum   = 0;

    power[0] = 1;            /* 2^(i-1) starts as 1 */

    for (idx_t i = 1; i <= M; ++i) {
        /* binomial (i+2 choose 3) = i(i+1)(i+2)/6 */
        __uint128_t t = (__uint128_t)(i + 2) * (i + 1) * i;
        uint64_t binom = (uint64_t)(t / 6);

        add_power_mul(sum_arr, &len_sum, power, len_power, binom);
        mul_small_inplace(power, &len_power, 2);   /* next power of 2 */
    }

    if (len_sum > 0)
        div_by_small(sum_arr, &len_sum, prime);

    /* Convert big integer to seq_t */
    if (len_sum == 0)
        return (seq_t)0;
    else if (len_sum == 1)
        return (seq_t)sum_arr[0];
    else {
        seq_t res = 0;
        for (int i = len_sum - 1; i >= 0; --i)
            res = (res << 64) + (seq_t)sum_arr[i];
        return res;
    }
}
