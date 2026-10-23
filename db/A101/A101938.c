/*
 * A101938  Numbers n with omega(n) < omega of 2 nearest larger and 2 nearest
 *          smaller neighbors.
 * https://oeis.org/A101938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 6233, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Finds numbers n such that ω(n) < ω(m) for all m in {n-2, n-1, n+1, n+2},
 * where ω(k) is the number of distinct prime factors of k.
 * The function pre‑computes all such numbers up to a safe upper bound
 * and returns the n‑th one (1‑based indexing).  Results are cached
 * and remain valid for any sequence of calls.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Get the n-th term of A101938 */
seq_t a(idx_t n);

/* ---------- implementation ---------- */

static bool seq_initialized = false;
static idx_t seq_len = 0;
static seq_t *seq_vals = NULL;

/* Maximum value up to which we sieve.  6 000 000 comfortably yields
   more than 400 000 terms, so it covers n up to at least 100 000. */
static const idx_t NMAX = 6000000;

/* Initialise the sequence: sieve ω and collect the qualifying numbers. */
static void seq_init(void)
{
    /* Allocate ω array (uint8_t is enough for ω up to this range). */
    uint8_t *omega = (uint8_t*)malloc((NMAX + 3) * sizeof(uint8_t));
    if (!omega) return;                    /* allocation failed */

    memset(omega, 0, (NMAX + 3) * sizeof(uint8_t));

    /* Compute ω via a simple prime sieve. */
    for (idx_t i = 2; i <= NMAX + 2; ++i) {
        if (omega[i] == 0) {               /* i is prime */
            for (idx_t j = i; j <= NMAX + 2; j += i)
                ++omega[j];
        }
    }

    /* Allocate space for the sequence.  Start with 200 000 slots. */
    idx_t cap = 200000;
    seq_vals = (seq_t*)malloc(cap * sizeof(seq_t));
    seq_len = 0;

    /* Scan all n >= 3; the condition uses neighbors at distance 1 and 2. */
    for (idx_t n = 3; n + 2 <= NMAX; ++n) {
        if (omega[n] < omega[n-2] &&
            omega[n] < omega[n-1] &&
            omega[n] < omega[n+1] &&
            omega[n] < omega[n+2]) {
            if (seq_len == cap) {          /* grow the array if needed */
                cap *= 2;
                seq_vals = (seq_t*)realloc(seq_vals, cap * sizeof(seq_t));
            }
            seq_vals[seq_len++] = (seq_t)n;
        }
    }

    seq_initialized = true;
}

/* Return the n-th term of the sequence (1‑based).  Undefined for n==0 or n > seq_len. */
seq_t a(idx_t n)
{
    if (!seq_initialized)
        seq_init();

    if (n == 0 || n > seq_len)
        return (seq_t)0;   /* out of range */

    return seq_vals[n - 1];
}
