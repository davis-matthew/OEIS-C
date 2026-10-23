/*
 * A139807  Number of distinct values of Product_{p is a part of P} (p-1) when
 *          P ranges over all partitions of n.
 * https://oeis.org/A139807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + number of partitions of n into parts of the form 2,
 *            5, or p+1 where p is a prime (excluding the part 12)
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------- static storage for primality ---------------- */
static uint64_t *primes = NULL;          /* list of primes so far      */
static size_t prime_cnt = 0, prime_cap = 0;
static uint64_t last_prime_checked = 1;  /* last number tested for primality */

/* --------------- static storage for parts ------------------- */
static uint32_t *parts = NULL;          /* list of distinct allowed parts */
static size_t parts_cnt = 0, parts_cap = 0;

/* --------------- array marking if a part value has been added */
static unsigned char *part_present = NULL;  /* 0 = not added, 1 = added */
static size_t part_present_cap = 0;

/* --------------- storage for the partition DP --------------- */
static seq_t *dp = NULL;          /* dp[0..max_n]   */
static size_t dp_size = 0;        /* allocated size */

/* -----------------------------------------------------------------
   Ensure that `primes` contains all primes ≤ limit (trial division
   incremental).  Uses static variables.
   ----------------------------------------------------------------- */
static void ensure_primes_upto(idx_t limit)
{
    if (limit < last_prime_checked)
        return;           /* already have what we need */

    if (prime_cap == 0) {
        prime_cap = 64;
        primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }

    for (uint64_t num = last_prime_checked + 1; num <= (uint64_t)limit; ++num) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > num)
                break;
            if (num % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                primes = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
            }
            primes[prime_cnt++] = num;
        }
    }
    last_prime_checked = (uint64_t)limit;
}

/* -----------------------------------------------------------------
   Add a new allowed part value `part` to the `parts` list and
   mark it as present.  Assumes `part_present` is large enough to
   index `part`.
   ----------------------------------------------------------------- */
static void add_part(uint32_t part)
{
    if (parts_cnt == parts_cap) {
        parts_cap = (parts_cap == 0) ? 64 : parts_cap << 1;
        parts = (uint32_t *)realloc(parts, parts_cap * sizeof(uint32_t));
    }
    parts[parts_cnt++] = part;
    if (part_present[part] == 0)
        part_present[part] = 1;
}

/* -----------------------------------------------------------------
   Main sequence function.
   ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* value for n=0 is unused, but safe */

    /* --------- make sure part_present array is big enough ------- */
    if ((size_t)n + 1 > part_present_cap) {
        size_t new_cap = (size_t)n + 1;
        part_present = (unsigned char *)realloc(part_present, new_cap);
        for (size_t i = part_present_cap; i < new_cap; ++i)
            part_present[i] = 0;
        part_present_cap = new_cap;
    }

    /* --------- ensure primes up to n-1 are known ----------------- */
    if (n > 1)
        ensure_primes_upto(n - 1);

    /* --------- add base parts 2 and 5 if they fit --------------- */
    if (n >= 2 && part_present[2] == 0)
        add_part(2);
    if (n >= 5 && part_present[5] == 0)
        add_part(5);

    /* --------- add prime+1 parts (excluding part 12) ------------ */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p == 11)          /* part 12 must be excluded */
            continue;
        uint32_t part = (uint32_t)(p + 1);
        if (part > (uint32_t)n)
            break; /* further primes produce larger parts */
        if (part_present[part] == 0)
            add_part(part);
    }

    /* --------- allocate/expand the dp array --------------------- */
    if ((size_t)n + 1 > dp_size) {
        dp = (seq_t *)realloc(dp, ((size_t)n + 1) * sizeof(seq_t));
        dp_size = (size_t)n + 1;
    }
    /* reset dp to zero except dp[0] */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* --------- compute partitions using the allowed parts ------- */
    for (size_t i = 0; i < parts_cnt; ++i) {
        uint32_t part = parts[i];
        if (part > (uint32_t)n)
            continue;
        for (idx_t j = part; j <= n; ++j)
            dp[j] = dp[j] + dp[j - part];
    }

    /* --------- distinct product values: add the product 0 case --- */
    return dp[n] + 1;
}
