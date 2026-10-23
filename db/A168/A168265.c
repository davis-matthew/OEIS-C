/*
 * A168265  a(n) = A003557(A060735(n)).
 * https://oeis.org/A168265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = offset within the concatenation of rows of numbers
 *            1..(prime(i)-1) where rows are indexed by i starting from 1
 * Range:     u8 n <= 1168, u16 n <= 9696, u32 n <= 9696, u64 n <= 9696,
 *            u128 n <= 9696, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is obtained by concatenating, for each i ≥ 1,
   the list 1,2,…,(prime(i)-1).  a(n) is the n‑th element of this
   concatenated list.  The implementation generates primes on demand
   and keeps a running cumulative sum of row lengths so that the
   correct row for any n can be found by binary search. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Prime number handling                                               */
/* ------------------------------------------------------------------ */

static uint32_t *primes = NULL;      /* array of primes in ascending order */
static size_t prime_cnt = 0;         /* number of stored primes */
static size_t prime_cap = 0;         /* array capacity */

static void grow_primes(size_t min_cap)
{
    if (prime_cap >= min_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2u : 16u;
    while (new_cap < min_cap) new_cap <<= 1;
    uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
    if (!tmp) abort();
    primes = tmp;
    prime_cap = new_cap;
}

/* Generates the next prime greater than the last stored prime. */
static uint32_t next_prime(void)
{
    if (prime_cnt == 0) return 2;
    uint32_t cand = primes[prime_cnt - 1] + 1;
    for (;; ++cand) {
        bool ok = true;
        for (size_t k = 0; k < prime_cnt; ++k) {
            uint32_t p = primes[k];
            if ((uint64_t)p * p > cand) break;
            if (cand % p == 0) { ok = false; break; }
        }
        if (ok) return cand;
    }
}

/* ------------------------------------------------------------------ */
/* Cumulative sum of row lengths                                      */
/* ------------------------------------------------------------------ */

static seq_t *cum = NULL;           /* cumulative sums: cum[i] = Σ_{j=1}^{i+1} (prime(j)-1) */
static size_t cum_cnt = 0;          /* number of cumulative values stored */
static size_t cum_cap = 0;          /* array capacity */

static void grow_cum(size_t min_cap)
{
    if (cum_cap >= min_cap) return;
    size_t new_cap = cum_cap ? cum_cap * 2u : 16u;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cum, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cum = tmp;
    cum_cap = new_cap;
}

/* Ensure the cumulative array is filled until the sum ≥ n. */
static void ensure_cumulative(idx_t n)
{
    seq_t target = (seq_t)n;
    while (cum_cnt == 0 || cum[cum_cnt - 1] < target) {
        uint32_t p = next_prime();
        grow_primes(prime_cnt + 1);
        primes[prime_cnt++] = p;

        seq_t row_len = (seq_t)p - 1;    /* length of this row */
        seq_t new_sum = row_len;
        if (cum_cnt > 0) new_sum += cum[cum_cnt - 1];

        grow_cum(cum_cnt + 1);
        cum[cum_cnt++] = new_sum;
    }
}

/* ------------------------------------------------------------------ */
/* Sequence function                                                   */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0; return 0 safely */
    ensure_cumulative(n);

    /* Binary search for first cumulative sum ≥ n. */
    size_t left = 0, right = cum_cnt - 1, row = cum_cnt;
    while (left <= right) {
        size_t mid = left + (right - left) / 2;
        if (cum[mid] >= (seq_t)n) {
            row = mid;
            if (mid == 0) break;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    seq_t prev_sum = (row == 0 ? (seq_t)0 : cum[row - 1]);
    return (seq_t)n - prev_sum;     /* offset inside the found row */
}
