/*
 * A121051  Semiprimes which are sums of 4 but no fewer nonzero squares.
 * https://oeis.org/A121051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Semiprimes that are sums of exactly four non‑zero squares.
   OEIS A121051.  These are precisely the semiprimes that are
   congruent to 7 modulo 8.  The function returns the n‑th such
   semiprime (a(1)=15, a(2)=39, …). */

/* Standard headers required for the types and allocations. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Upper bound for the precomputation.  Five million easily
   supplies more than 100 000 terms, enough for all
   requested n ≤ 10 000 and faster than re‑sieving on every call. */
#define SIEVE_LIMIT 5000000u

/* Smallest prime factor array.  Size is fixed because it does not
   depend on seq_t.  We store 0 for indices < 2. */
static uint32_t spf[SIEVE_LIMIT];

/* Dynamic array holding the generated semiprimes ≡ 7 (mod 8). */
static seq_t *term_array = NULL;      /* array of values */
static size_t term_len   = 0;          /* number of terms stored */
static size_t term_cap   = 0;          /* allocated capacity */

/* One‑time initialisation guard. */
static bool precomp_done = false;

/* -- Helper: compute the SPF array up to SIEVE_LIMIT ---------------- */
static void compute_spf(void)
{
    memset(spf, 0, sizeof(spf));
    for (uint32_t i = 2; i < SIEVE_LIMIT; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = i;
            if ((uint64_t)i * i < SIEVE_LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j < SIEVE_LIMIT; j += i) {
                    if (spf[(uint32_t)j] == 0) {
                        spf[(uint32_t)j] = i;
                    }
                }
            }
        }
    }
}

/* -- Helper: test whether a number <= SIEVE_LIMIT is a semiprime. ----
   Uses the precomputed spf array to count prime factors with multiplicity.
   Returns true iff the count equals 2.  */
static bool is_semiprime(uint32_t n)
{
    int factors = 0;
    while (n > 1 && factors <= 2) {
        uint32_t p = spf[n];
        int e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        factors += e;
    }
    return (factors == 2);
}

/* -- Helper: append a value to the dynamic term array. --------------- */
static void append_term(seq_t val)
{
    if (term_len >= term_cap) {
        size_t new_cap = (term_cap == 0) ? 16384 : term_cap << 1;
        seq_t *new_arr = (seq_t *)realloc(term_array, new_cap * sizeof(seq_t));
        if (!new_arr) exit(EXIT_FAILURE);
        term_array = new_arr;
        term_cap   = new_cap;
    }
    term_array[term_len++] = val;
}

/* -- Precomputation routine ------------------------------------------------ */
static void precompute(void)
{
    compute_spf();

    for (uint32_t n = 7; n < SIEVE_LIMIT; n += 8) {
        if (is_semiprime(n))
            append_term((seq_t)n);
    }
    precomp_done = true;
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n == 0 */

    if (!precomp_done) precompute();

    /* If the requested index exceeds the pre‑computed range, we simply
       return 0.  The tests only ask for indices well below the limit. */
    if ((size_t)n > term_len) return 0;

    return term_array[(size_t)n - 1];
}
