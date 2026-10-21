/*
 * A077608  Number of compositions of n into twin primes (i.e., primes that
 *          are members of a twin prime pair, like 3, 5, 7, 11, 13, but not 2
 *          or 23).
 * https://oeis.org/A077608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into twin primes such that
 *            each part is a prime that belongs to a twin‑prime pair
 *            (3,5,7,11,13,…). Recurrence: a(0)=1; for n>0, a(n)=∑_{t∈T, t≤n}
 *            a(n−t) where T = {twin primes ≤ n}
 * Range:     u8 n <= 27, u16 n <= 48, u32 n <= 91, u64 n <= 176,
 *            u128 n <= 346, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- prime sieve utilities ---------- */
static uint8_t *primes_arr = NULL;  /* 1 = prime, 0 = composite */
static idx_t prime_limit = 0;

/* Compute primes up to 'limit' (inclusive) using a simple OEIS sieve. */
static void ensure_prime_limit(idx_t limit)
{
    if (prime_limit >= limit) return;

    uint8_t *arr = (uint8_t *)malloc((size_t)(limit + 1));
    if (!arr) return; /* out of memory, caller must handle */
    for (idx_t i = 0; i <= limit; ++i) arr[i] = 1;
    arr[0] = arr[1] = 0;
    for (idx_t i = 2; (idx_t)i * i <= limit; ++i)
    {
        if (arr[i])
            for (idx_t j = i * i; j <= limit; j += i)
                arr[j] = 0;
    }
    free(primes_arr);
    primes_arr = arr;
    prime_limit = limit;
}

/* ---------- twin prime utilities ---------- */
static uint32_t *twin_primes_arr = NULL;   /* list of twin primes <= current limit */
static size_t twin_primes_cap = 0;
static size_t twin_primes_cnt = 0;
static idx_t twin_limit = 0;               /* maximum n for which twin list is valid */

/* Rebuild twin prime list up to 'limit'. */
static void ensure_twin_primes(idx_t limit)
{
    if (twin_limit >= limit) return;

    /* ensure prime table covers p±2 for every p≤limit */
    ensure_prime_limit(limit + 2);

    free(twin_primes_arr);
    twin_primes_cap = limit / 2 + 10;
    twin_primes_arr = (uint32_t *)malloc(twin_primes_cap * sizeof(uint32_t));
    twin_primes_cnt = 0;

    for (idx_t p = 3; p <= limit; ++p)
    {
        if (!primes_arr[p]) continue;
        bool twin = false;
        if (p > 2 && primes_arr[p - 2]) twin = true;
        if (!twin && primes_arr[p + 2]) twin = true;
        if (twin)
        {
            if (twin_primes_cnt >= twin_primes_cap)
            {
                twin_primes_cap *= 2;
                twin_primes_arr = (uint32_t *)realloc(twin_primes_arr, twin_primes_cap * sizeof(uint32_t));
            }
            twin_primes_arr[twin_primes_cnt++] = (uint32_t)p;
        }
    }
    twin_limit = limit;
}

/* ---------- DP utilities ---------- */
static seq_t *dp_arr = NULL;   /* DP table: a[0 .. dp_len-1] */
static size_t dp_cap = 0;
static idx_t dp_len = 0;       /* how many entries are computed (dp_len>0 means dp[0] exists) */

/* Ensure DP table computed up to index 'n'. */
static void ensure_dp(idx_t n)
{
    if (dp_len > n) return;              /* already available */

    /* extend storage if necessary */
    if (!dp_arr)
    {
        dp_cap = n + 1;
        dp_arr = (seq_t *)malloc(dp_cap * sizeof(seq_t));
    }
    else if (dp_cap <= (size_t)n)
    {
        size_t new_cap = dp_cap * 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        dp_arr = (seq_t *)realloc(dp_arr, new_cap * sizeof(seq_t));
        dp_cap = new_cap;
    }

    /* initialise base case if needed */
    if (dp_len == 0)
    {
        dp_arr[0] = (seq_t)1;
        dp_len = 1;
    }

    /* compute missing values */
    for (idx_t i = dp_len; i <= n; ++i)
    {
        seq_t sum = (seq_t)0;
        for (size_t j = 0; j < twin_primes_cnt && twin_primes_arr[j] <= i; ++j)
            sum += dp_arr[(size_t)(i - twin_primes_arr[j])];
        dp_arr[(size_t)i] = sum;
    }
    dp_len = n + 1;
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    /* Step 1: ensure twin primes up to n are known */
    ensure_twin_primes(n);

    /* Step 2: compute DP up to n */
    ensure_dp(n);

    /* Step 3: return result */
    return dp_arr[(size_t)n];
}
