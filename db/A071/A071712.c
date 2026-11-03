/*
 * A071712  The 23-step cycle reached for any initial value k less than
 *          100000, after iterations of sopf(8x+1), where sopf(n) denote the
 *          sum of the prime factors of n (sopf(12) = 2+2+3 = 7).
 * https://oeis.org/A071712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: full, pass, 23 terms through n = 23 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the 23‑cycle of the map x → sopf(8x+1).  a(n) returns the n‑th
   element of that cycle (with 1‑based indexing).  The sequence is
   independent of the starting value k<100000. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ----------  helper: sum of prime factors with multiplicity  ----------
   The numbers that appear in this sequence are all < 10^6, so a simple
   trial‑division factorisation fits easily in 64 bit.  The function
   returns a 64‑bit value; it is then cast to seq_t when stored.          */
static uint64_t sopf64(uint64_t n)
{
    uint64_t sum = 0;

    /* factor 2 */
    while ((n & 1ULL) == 0) {
        sum += 2;
        n >>= 1;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        while (n % p == 0) {
            sum += p;
            n /= p;
        }
    }

    if (n > 1)   /* remaining prime factor */
        sum += n;

    return sum;
}

#define CYCLE_LEN 23

static seq_t g_cycle[CYCLE_LEN];
static bool g_initialized = false;

/* one‑time construction of the 23‑element cycle, starting with 66 */
static void init_cycle(void)
{
    uint64_t cur = 66ULL;
    for (size_t i = 0; i < CYCLE_LEN; ++i) {
        g_cycle[i] = (seq_t)cur;
        uint64_t next = sopf64(8ULL * cur + 1ULL);
        cur = next;
    }
    g_initialized = true;
}

/* public sequence function, external linkage */
seq_t a(idx_t n)
{
    if (!g_initialized)
        init_cycle();

    /* 1‑based indexing, cycle length is 23 */
    size_t idx = (size_t)((n - 1ULL) % CYCLE_LEN);
    return g_cycle[idx];
}
