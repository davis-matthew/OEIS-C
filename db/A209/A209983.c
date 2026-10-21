/*
 * A209983  (A209982)/2.
 * https://oeis.org/A209983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16 * Σ_{k=1}^n φ(k) - 6 for n ≥ 1, a(0) = 0 where φ is
 *            Euler's totient function
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static storage for the sieve and prefix sums */
static uint64_t *phi_arr   = NULL;
static uint64_t *pref_arr  = NULL;
static idx_t    computed_upto = 0;   /* largest index for which arrays are valid */

static void build_sieve(idx_t up)
{
    if (up <= computed_upto) return;          /* already computed up to at least 'up' */

    /* allocate new arrays */
    size_t sz = (size_t)up + 1;
    uint64_t *new_phi = (uint64_t*)malloc(sz * sizeof(uint64_t));
    uint64_t *new_pref= (uint64_t*)malloc(sz * sizeof(uint64_t));
    if (!new_phi || !new_pref) {                /* allocation failure – abort */
        abort();
    }

    /* initialise phi[i] = i */
    for (size_t i = 0; i < sz; ++i) new_phi[i] = (uint64_t)i;

    /* sieve to compute totients */
    for (size_t i = 2; i < sz; ++i) {
        if (new_phi[i] == (uint64_t)i) {        /* i is prime */
            for (size_t j = i; j < sz; j += i) {
                new_phi[j] -= new_phi[j] / i;
            }
        }
    }
    /* prefix sums of φ */
    new_pref[0] = 0;
    for (size_t i = 1; i < sz; ++i)
        new_pref[i] = new_pref[i-1] + new_phi[i];

    /* free old arrays */
    free(phi_arr);
    free(pref_arr);

    phi_arr   = new_phi;
    pref_arr  = new_pref;
    computed_upto = up;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* ensure sieve up to n */
    build_sieve(n);

    /* a(n) = 16 * Σ φ(k) - 6  for n ≥ 1 */
    uint64_t val = pref_arr[(size_t)n] * 16ULL - 6ULL;
    return (seq_t)val;
}
