/*
 * A106764  Primes with digit sum = 26.
 * https://oeis.org/A106764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 461, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A106764 – primes whose digit sum is 26.
    Formula: a(n) is the n-th prime p with digit_sum(p)=26.
    (The OEIS index starts at n=1.)  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------
   helper: digit sum of an unsigned 64‑bit integer
   -------------------------------------------------------------------- */
static inline int digit_sum_uint64(uint64_t v)
{
    int s = 0;
    while (v) {
        s += (int)(v % 10);
        v /= 10;
    }
    return s;
}

/* --------------------------------------------------------------------
   static storage for the pre‑computed sequence
   -------------------------------------------------------------------- */
static bool          initialized = false;   /* init flag           */
static seq_t        *seq_arr     = NULL;   /* array of primes with sum 26 */
static size_t        seq_len     = 0;      /* current number of terms  */
static size_t        seq_cap     = 0;      /* allocated capacity        */

/* --------------------------------------------------------------------
   one‑time construction of the sequence up to a safe upper bound
   (10 000 000 guarantees at least 10 000 terms).
   -------------------------------------------------------------------- */
static void init_sequence(void)
{
    if (initialized) return;

    const uint32_t LIMIT = 10000000U;          /* upper bound to sieve */

    /* Explicit casts required so the code compiles as C++ */
    uint8_t *sieve = (uint8_t*)malloc((size_t)LIMIT + 1);
    if (!sieve) abort();                      /* memory failure          */
    memset(sieve, 0, (size_t)LIMIT + 1);

    /* simple Eratosthenes sieve */
    for (uint32_t p = 2; (uint64_t)p * p <= LIMIT; ++p) {
        if (!sieve[p]) {
            for (uint32_t q = p * p; q <= LIMIT; q += p)
                sieve[q] = 1;
        }
    }

    /* collect primes with digit sum 26 */
    for (uint32_t n = 2; n <= LIMIT; ++n) {
        if (!sieve[n]) {
            if (digit_sum_uint64((uint64_t)n) == 26) {
                if (seq_len == seq_cap) {
                    seq_cap = seq_cap ? seq_cap * 2 : 64;
                    seq_arr = (seq_t*)realloc(seq_arr, seq_cap * sizeof(seq_t));
                    if (!seq_arr) abort();
                }
                seq_arr[seq_len++] = (seq_t)n;
            }
        }
    }

    free(sieve);
    initialized = true;
}

/* --------------------------------------------------------------------
   public function required by the test harness
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* No assumption on monotonicity of successive calls */
    init_sequence();

    /* Guard against out‑of‑range requests */
    if ((size_t)n == 0 || (size_t)n > seq_len)
        abort();   /* programmer error; no valid answer */

    return seq_arr[n - 1];
}
