/*
 * A106141  Semiprimes (mod 5).
 * https://oeis.org/A106141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the residue modulo 5 of the nth semiprime (sequence A106141). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Cached data ----------
 *  spf      – array of smallest prime factor for each integer
 *  spf_lim  – current maximum index of spf
 *  res      – dynamic array of previously computed residues
 *  res_cap  – current capacity of res
 *  last_chk – the integer from which the next scan starts
 *  res_size – number of residues stored (equals the number of semiprimes found)
 * ---------- */
static uint64_t *spf     = NULL;
static size_t   spf_lim  = 0;

static seq_t   *res      = NULL;
static size_t   res_cap  = 0;
static idx_t    res_size = 0;

static uint64_t last_chk = 4;   /* next integer to test */

/* ---------- Helper: ensure spf array up to 'limit' ----------
 *   Re‑allocates and recomputes the spf array if 'limit' exceeds the
 *   current spf_lim.  The array is indexed from 0 to limit inclusive. */
static void ensure_spf(size_t limit)
{
    if (limit <= spf_lim)
        return;

    uint64_t *new_spf = (uint64_t*)calloc(limit + 1, sizeof(uint64_t));
    if (!new_spf)
        return;          /* allocation failed – will cause a later failure */

    new_spf[0] = 0;
    new_spf[1] = 1;
    for (size_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {                /* i is prime */
            new_spf[i] = (uint64_t)i;
            uint64_t ii = (uint64_t)i * (uint64_t)i;
            if (ii <= (uint64_t)limit)
                for (uint64_t j = ii; j <= (uint64_t)limit; j += (uint64_t)i)
                    if (new_spf[j] == 0)
                        new_spf[j] = (uint64_t)i;
        }
    }

    free(spf);
    spf     = new_spf;
    spf_lim = limit;
}

/* ---------- Helper: is this number a semiprime? ----------
 *   Uses the pre‑computed spf array.  Returns true iff the number
 *   has exactly two prime factors when multiplicity is counted. */
static bool is_semiprime(uint64_t x)
{
    if (x < 4)
        return false;          /* first semiprime is 4 */

    int count = 0;
    while (x > 1) {
        uint64_t p = spf[x];
        ++count;
        if (count > 2)
            return false;      /* more than two factors */
        x /= p;
    }
    return count == 2;
}

/* ---------- Main sequence function ----------
 *   Computes a(n) for n>=1.  Results are memoised: the first
 *   n‑th semiprime residues are stored in a static array, so
 *   subsequent calls are fast regardless of order. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;              /* undefined for n<1 */

    /* Generate residues until we have at least n of them. */
    while (res_size < n) {
        /* Ensure the SPF array is large enough for the current scan. */
        if (spf_lim == 0)
            ensure_spf(2000);
        size_t limit = spf_lim;

        /* Scan from the last checked integer up to the current limit. */
        for (uint64_t x = last_chk; x <= limit; ++x) {
            if (is_semiprime(x)) {
                /* Grow the result array if necessary. */
                if (res_size == res_cap) {
                    size_t new_cap = res_cap ? res_cap * 2 : 64;
                    seq_t *new_res = (seq_t*)calloc(new_cap, sizeof(seq_t));
                    if (!new_res)
                        return 0;          /* allocation failed */
                    if (res) {
                        for (idx_t i = 0; i < res_size; ++i)
                            new_res[i] = res[i];
                        free(res);
                    }
                    res     = new_res;
                    res_cap = new_cap;
                }
                res[res_size++] = (seq_t)(x % 5);
            }
        }

        last_chk = limit + 1;          /* next scan starts after the limit */

        /* Extend the SPF array for the next round if needed. */
        size_t new_limit = spf_lim * 2;
        if (new_limit < spf_lim + 1000)
            new_limit = spf_lim + 1000;
        ensure_spf(new_limit);
    }

    return res[n - 1];
}
