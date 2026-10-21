/*
 * A077661  Triangle read by rows in which the n-th row contains n smallest
 *          numbers not coprime to n; except first row = {1}.
 * https://oeis.org/A077661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabl
 * Formula:   not stated
 * Range:     u8 n <= 151, u16 n <= 9949, u32 n <= 9949, u64 n <= 9949,
 *            u128 n <= 9949, big any n
 * Helpers:   row_and_pos_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A077661 – Triangle read by rows.
    Row r (r≥2) contains the r smallest positive integers that are not
    coprime to r (i.e. share a common divisor >1), written in ascending
    order with duplicates removed.  Row 1 is {1}.
    The sequence a(n) enumerates this triangle row by row.  For a given
    index n the function finds the row r such that r*(r+1)/2 ≥ n,
    determines the position within that row, and outputs the required
    element.  The construction of a row uses a small heap that merges
    the arithmetic progressions of the distinct prime divisors of r.
    Complexity per query: O(r log k) with k–the number of distinct prime
    divisors of r; for n≤10000 r≤141 so the routine is easily fast. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/row_and_pos_idx.h"

/* pre‑computed primes up to a fixed limit (sufficient for all rows needed) */
#define SIEVE_LIMIT 200000
static unsigned char prime_cache[SIEVE_LIMIT + 1];
static unsigned char primes_arr[SIEVE_LIMIT];
static size_t primes_cnt = 0;
static bool primes_init = false;

static void init_primes(void)
{
    if (primes_init) return;
    primes_init = true;
    for (int i = 2; i <= SIEVE_LIMIT; ++i)
        prime_cache[i] = 0;
    size_t cnt = 0;
    for (int i = 2; i <= SIEVE_LIMIT; ++i)
    {
        if (!prime_cache[i])
        {
            primes_arr[cnt++] = (unsigned char)i;
            if ((int64_t)i * i <= SIEVE_LIMIT)
                for (int64_t j = (int64_t)i * i; j <= SIEVE_LIMIT; j += i)
                    prime_cache[j] = 1;
        }
    }
    primes_cnt = cnt;
}

/* compute the value of a(n) */
seq_t a(uint64_t n)
{
    uint64_t r, pos;
    row_and_pos_idx(n, &r, &pos);

    /* special row 1 */
    if (r == 1) return (seq_t)1;

    init_primes();

    /* factor r, collect distinct prime factors */
    uint64_t temp = r;
    uint64_t prime_factors[20];
    size_t k = 0;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes_arr[i] * primes_arr[i] <= temp; ++i)
    {
        uint64_t p = primes_arr[i];
        if (temp % p == 0)
        {
            prime_factors[k++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) prime_factors[k++] = temp;

    /* heap of current multiples for each prime: use simple linear search */
    seq_t heap_vals[20];
    for (size_t i = 0; i < k; ++i)
        heap_vals[i] = (seq_t)prime_factors[i];

    seq_t last_val = 0;
    bool first = true;
    uint64_t seen = 0;

    while (1)
    {
        /* find minimum in heap */
        size_t min_i = 0;
        seq_t min_val = heap_vals[0];
        for (size_t i = 1; i < k; ++i)
            if (heap_vals[i] < min_val)
            {
                min_val = heap_vals[i];
                min_i = i;
            }

        /* a new distinct value? */
        if (first || min_val != last_val)
        {
            ++seen;
            if (seen == pos) return min_val;
            last_val = min_val;
            first = false;
        }
        /* advance this prime */
        heap_vals[min_i] += (seq_t)prime_factors[min_i];
    }
}
