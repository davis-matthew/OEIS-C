/*
 * A095749  Square array A(row>=1, col>=1) by antidiagonals: A(r,c) contains
 *          the c:th prime p for which A037888(p)=(r-1).
 * https://oeis.org/A095749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   bitlen_seq_t, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A095749:
 * a(n) is the prime at column c in row r of the square array A,
 * where (r,c) are the antidiagonal coordinates of n.
 * A(r,c) is the c‑th prime p such that the minimum number of bit flips
 * needed to make binary(p) a palindrome equals r‑1   (A037888(p)=r‑1). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/bitlen_seq_t.h"

/* Maximum flip count needed for n up to 10000 (about 141).
 * We allow a small margin. */
#define MAX_FLIPS            256
/* Maximum number of primes we will keep per flip bucket. */
#define MAX_PRIMES_PER_FLIP  256

static seq_t primes_by_flips[MAX_FLIPS][MAX_PRIMES_PER_FLIP];
static size_t len_by_flips[MAX_FLIPS] = {0};

static uint64_t next_prime_candidate = 2;

/* Compute the number of bit flips required to make the binary
 * representation of p a palindrome. */
static size_t flips_needed(seq_t p)
{
    unsigned int len = bitlen_seq_t(p);
    size_t flips = 0;
    idx_t i = 0;
    idx_t j = (idx_t)len - 1;
    while (i < j)
    {
        bool left  = ((p >> i) & (seq_t)1) != 0;
        bool right = ((p >> j) & (seq_t)1) != 0;
        if (left != right)
            ++flips;
        ++i;
        --j;
    }
    return flips;
}

/* Generate primes until bucket f contains at least 'need' primes. */
static void generate_until(size_t f, size_t need)
{
    if (f >= MAX_FLIPS)
        return;                       /* should never happen for valid input */

    while (len_by_flips[f] < need)
    {
        uint64_t cand = next_prime_candidate;
        while (!is_prime_small(cand))
            ++cand;
        next_prime_candidate = cand + 1;

        seq_t prime = (seq_t)cand;
        size_t f_needed = flips_needed(prime);
        if (f_needed >= MAX_FLIPS)
            continue;                 /* ignore primes that would exceed table */
        if (len_by_flips[f_needed] < MAX_PRIMES_PER_FLIP)
            primes_by_flips[f_needed][len_by_flips[f_needed]++] = prime;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;               /* undefined for n < 1 */

    /* Find the antidiagonal d such that T(d-1) < n ≤ T(d) (T(k)=k(k+1)/2). */
    idx_t d = 1;
    while ((d * (d + 1)) / 2 < n)
        ++d;

    idx_t prev_total   = (d - 1) * d / 2;
    idx_t pos_in_diag  = n - prev_total;      /* 1 … d */
    idx_t r = pos_in_diag;
    idx_t c = d - r + 1;

    size_t flips_needed_cnt = (size_t)(r - 1);
    generate_until(flips_needed_cnt, (size_t)c);

    return primes_by_flips[flips_needed_cnt][c - 1];
}
