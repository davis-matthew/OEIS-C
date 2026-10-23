/*
 * A198696  Positions of local maxima in differences of primes, A001223.
 * https://oeis.org/A198696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes positions of local maxima in differences of primes (A001223). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Dynamic storage for primes, gaps, and positions. */
static idx_t *primes = NULL;
static size_t primes_cnt = 0, primes_cap = 0;

static idx_t *gaps = NULL;
static size_t gaps_cnt = 0, gaps_cap = 0;

static idx_t *positions = NULL;
static size_t pos_cnt = 0, pos_cap = 0;

/* Ensure the array @arr has room for at least @need more elements. */
static void
ensure_capacity(size_t *cap, size_t *cnt, size_t need,
                void **arr, size_t elem_size)
{
    if (*cnt + need > *cap) {
        size_t new_cap = (*cap == 0) ? 64 : *cap * 2;
        while (new_cap < *cnt + need)
            new_cap *= 2;
        *arr = realloc(*arr, new_cap * elem_size);
        *cap = new_cap;
    }
}

static void
add_prime(idx_t p)
{
    ensure_capacity(&primes_cap, &primes_cnt, 1,
                    (void **)&primes, sizeof(idx_t));
    primes[primes_cnt++] = p;
}

static void
add_gap(idx_t g)
{
    ensure_capacity(&gaps_cap, &gaps_cnt, 1,
                    (void **)&gaps, sizeof(idx_t));
    gaps[gaps_cnt++] = g;
}

static void
add_position(idx_t pos)     /* pos is 1‑based (gap index) */
{
    ensure_capacity(&pos_cap, &pos_cnt, 1,
                    (void **)&positions, sizeof(idx_t));
    positions[pos_cnt++] = pos;
}

/* Trial division using the primes grown so far. */
static bool
is_prime_candidate(idx_t cand)
{
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if (p * p > cand)
            break;
        if (cand % p == 0)
            return false;
    }
    return true;
}

/* Initialise with the first two primes and the initial gap. */
static void
init_primes(void)
{
    if (primes_cnt == 0) {
        add_prime(2);
        add_prime(3);
        add_gap(1);          /* 3 – 2 */
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n = 0; keep behaviour stable */

    init_primes();

    /* Keep generating primes until we have found n local maxima. */
    while (pos_cnt < (size_t)n) {
        /* Find next prime after the last one stored. */
        idx_t last = primes[primes_cnt - 1];
        idx_t cand = last + 1;
        if (cand < 4)
            cand = 5;
        if ((cand & 1) == 0)
            ++cand;

        for (;; cand += 2) {
            if (is_prime_candidate(cand))
                break;
        }

        add_prime(cand);
        /* The new gap is the difference to the previous prime. */
        idx_t new_gap = cand - primes[primes_cnt - 2];
        add_gap(new_gap);

        /* When at least three gaps exist, the middle one may be a local maximum. */
        if (gaps_cnt >= 3) {
            idx_t g_prev = gaps[gaps_cnt - 3];
            idx_t g_curr = gaps[gaps_cnt - 2];
            idx_t g_next = gaps[gaps_cnt - 1];
            if (g_prev < g_curr && g_curr > g_next)
                add_position(gaps_cnt - 1);  /* 1‑based index of the middle gap */
        }
    }

    /* Positions are stored 1‑based; return the n‑th one. */
    return (seq_t)positions[n - 1];
}
