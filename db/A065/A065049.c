/*
 * A065049  Odd primes of incorrect parity: number of 1's in the binary
 *          representation of n (mod 2) == 1 - (n mod 3) (mod 2). Also called
 *          isolated primes.
 * https://oeis.org/A065049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th odd prime p for which * popcount(p) % 2 == (1 -
 *            (p % 3)) & 1 * (i.e., popcount parity is 1 when p%3≠1 and 0 when
 *            p%3==1)
 * Range:     u8 n <= 18, u16 n <= 2942, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ---------- utility: dynamic array capacity ----------
 * Ensures that *ptr points to an array of at least 'need' elements
 * of size 'elem_size'.  If reallocation is required, the array
 * is grown to a power of two large enough to hold 'need'.
 */
static void ensure_cap(size_t *cap, size_t need, void **ptr, size_t elem_size)
{
    if (need <= *cap) return;
    size_t new_cap = *cap ? *cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    *ptr = realloc(*ptr, new_cap * elem_size);
    *cap = new_cap;
}

/* ---------- popcount parity --------------------------------
 * Returns 0 if the number of 1‑bits in x is even, 1 otherwise.
 * Works for any unsigned integer type that can be held in idx_t.
 */
static int popcount_parity(idx_t x)
{
    int p = 0;
    while (x) {
        p ^= 1;
        x &= x - 1;
    }
    return p;
}

/* ---------- prime generation --------------------------------
 * Generates odd primes up to arbitrary size by trial division
 * using the list of previously found primes.
 */
static idx_t *g_primes = NULL;
static size_t g_primes_cap = 0, g_primes_size = 0;
static idx_t g_last_checked = 1;

// Helper to add a prime to the list
static void push_prime(idx_t p)
{
    ensure_cap(&g_primes_cap, g_primes_size + 1,
               (void **)&g_primes, sizeof(idx_t));
    g_primes[g_primes_size++] = p;
}

static void g_generate_next_prime(void)
{
    idx_t cand = g_last_checked + 1;
    while (1) {
        int is_prime = 1;
        for (size_t i = 0; i < g_primes_size; ++i) {
            idx_t p = g_primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            g_last_checked = cand;
            push_prime(cand);
            return;
        }
        ++cand;
    }
}

/* ---------- sequence generation --------------------------------
 * a(idx_t n) returns the n-th term (1‑based) of A065049.
 */
static idx_t *seq_cache = NULL;
static size_t seq_cap = 0, seq_size = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 // offset is 1

    /* generate enough terms */
    while (seq_size < n) {
        g_generate_next_prime();          // ensure next prime available
        idx_t prime = g_primes[g_primes_size - 1];

        /* we only consider odd primes; skip 2 */
        if (prime == 2) continue;

        int pc_par = popcount_parity(prime);
        int r = prime % 3;
        int required = (r == 1) ? 0 : 1;          // popcount parity must be this

        if (pc_par == required) {
            ensure_cap(&seq_cap, seq_size + 1,
                       (void **)&seq_cache, sizeof(idx_t));
            seq_cache[seq_size++] = prime;
        }
    }

    /* cast to seq_t – if the returned value cannot be represented in
     * seq_t it will overflow, which is acceptable – only terms that
     * fit in seq_t are expected to be checked. */
    return (seq_t)seq_cache[n - 1];
}
