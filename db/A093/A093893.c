/*
 * A093893  Numbers n such that every sum of two or more divisors is
 *          composite.
 * https://oeis.org/A093893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A093893: odd integers n such that every sum of two or more
 * divisors of n is composite.  The function memoizes previously-found
 * terms and works for any index n≥1, independent of call order. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*--- Utility: primality test for 64‑bit integers --------------------------------*/
static bool is_prime_u64(uint64_t n)
{
    if (n < 2)            return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0)     return false;
    if (n % 3 == 0)       return false;
    for (uint64_t i = 5; i * i <= n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    return true;
}

/*--- Recursively search for a prime sum of a subset of divisors ---------------*/
static void rec_subset(size_t idx, unsigned __int128 sum, size_t count,
                       const uint64_t *divs, size_t dcnt, bool *found)
{
    if (*found) return;
    if (idx == dcnt) {
        if (count >= 2) {
            if (sum <= (unsigned __int128)UINT64_MAX &&
                is_prime_u64((uint64_t)sum))
                *found = true;
        }
        return;
    }
    /* Do not take divs[idx] */
    rec_subset(idx + 1, sum, count, divs, dcnt, found);
    /* Take divs[idx] */
    rec_subset(idx + 1, sum + divs[idx], count + 1, divs, dcnt, found);
}

/*--- Test whether a candidate satisfies the definition ------------------------*/
static bool candidate_ok(uint64_t m)
{
    if (m == 1)                     return true;      /* 1 is a trivial member */
    if (is_prime_u64(m))            return true;      /* all odd primes */

    /* Enumerate all divisors of the odd number m */
    uint64_t divs[4096];            /* generous static array */
    size_t   dcnt = 0;

    for (uint64_t i = 1; i * i <= m; i += 2) {        /* m is odd, so i odd */
        if (m % i == 0) {
            divs[dcnt++] = i;
            uint64_t j = m / i;
            if (j != i) {
                divs[dcnt++] = j;
                if (dcnt >= sizeof(divs) / sizeof(divs[0])) break; /* safety */
            }
        }
    }

    bool found_prime_sum = false;
    rec_subset(0, 0, 0, divs, dcnt, &found_prime_sum);
    return !found_prime_sum;
}

/*--- Dynamic cache of computed sequence values --------------------------------*/
static seq_t *seq   = NULL;   /* array of terms */
static size_t  slen  = 0;      /* number of stored terms */
static size_t  scap  = 0;      /* allocated capacity */

/* Append a new value to the dynamic array, resizing as needed */
static void append_value(seq_t val)
{
    if (slen == scap) {
        size_t new_cap = (scap == 0) ? 16 : scap << 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – abort gracefully */
            abort();
        }
        seq = tmp;
        scap = new_cap;
    }
    seq[slen++] = val;
}

/*--- The required sequence function -------------------------------------------*/
seq_t a(idx_t n)
{
    static uint64_t next_candidate = 1;   /* start with 1 (odd) */

    /* Recompute up to the required index */
    while (slen < (size_t)n) {
        if (candidate_ok(next_candidate))
            append_value((seq_t)next_candidate);
        next_candidate += 2;            /* skip evens – all terms are odd */
    }
    return seq[n - 1];
}
