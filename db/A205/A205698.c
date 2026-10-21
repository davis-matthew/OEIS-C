/*
 * A205698  Numbers k for which 7 divides prime(k)-prime(j) for some j<k; each
 *          k occurs once for each such j.
 * https://oeis.org/A205698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that * Σ_{t=1}^{k} c_t ≥ n, where c_t =
 *            #{j < t | prime(t) ≡ prime(j) (mod 7)}. * Each index k repeats
 *            once for each earlier prime with the same residue * modulo 7.
 *            This implementation builds primes sequentially, keeps the *
 *            running count of residues mod 7, and stores cumul...
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"

/* static state */
static uint64_t lastPrime = 0;      /* last prime produced */
static size_t   primes_len = 0;     /* number of primes generated */
static uint64_t *cum = NULL;       /* cumulative counts of indices */
static size_t   cum_len = 0;        /* number of elements in cum */
static size_t   cum_cap = 0;        /* capacity of cum array */
static size_t   residue_count[7] = {0};
static bool     initialized = false;

/* initialise with first prime 2 */
static void init(void)
{
    if (initialized)
        return;

    /* cumulative array starts with element 0 */
    ensure_capacity(1, &cum_cap, (void **)&cum, sizeof(uint64_t));
    cum[0] = 0;
    cum_len = 1;

    /* first prime 2 */
    lastPrime = 2;
    primes_len = 1;
    size_t r = 2 % 7;
    size_t c = residue_count[r];            /* c_1 = 0 */
    ensure_capacity(cum_len + 1, &cum_cap, (void **)&cum, sizeof(uint64_t));
    cum[cum_len] = cum[cum_len - 1] + (uint64_t)c;
    cum_len++;
    residue_count[r]++;                      /* record prime 2 */

    initialized = true;
}

/* generate the next prime after the lastPrime and update state */
static void generate_next_prime(void)
{
    uint64_t candidate = lastPrime + 1;
    if (candidate <= 2)
        candidate = 3;
    if ((candidate & 1) == 0)
        candidate++;                         /* make it odd */

    while (!is_prime((idx_t)candidate))
        candidate += 2;

    /* new prime found: compute its residue and cumulative count */
    size_t r = candidate % 7;
    size_t c = residue_count[r];             /* number of previous primes with same residue */

    ensure_capacity(cum_len + 1, &cum_cap, (void **)&cum, sizeof(uint64_t));
    cum[cum_len] = cum[cum_len - 1] + (uint64_t)c;
    cum_len++;

    residue_count[r]++;                       /* prime now counted for future residues */
    lastPrime = candidate;
    primes_len++;
}

/* ensure cum has at least n entries in its cumulative counts */
static void extend_to(idx_t n)
{
    while (cum[cum_len - 1] < (uint64_t)n)
        generate_next_prime();
}

/* required OEIS function */
seq_t a(idx_t n)
{
    if (!initialized)
        init();

    if (n == 0u)
        return 0;                     /* undefined, but harmless */

    extend_to(n);

    /* binary search for smallest k with cum[k] >= n */
    size_t left = 1;
    size_t right = cum_len - 1;        /* cum[0] is 0, so k starts at 1 */
    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (cum[mid] >= (uint64_t)n)
            right = mid;
        else
            left = mid + 1;
    }
    return (seq_t)left;
}
