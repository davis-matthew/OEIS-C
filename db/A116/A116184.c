/*
 * A116184  Numbers n such that 37^3 divides the numerator of generalized
 *          harmonic number H(36,n) = Sum[ 1/k^n, {k,1,36} ].
 * https://oeis.org/A116184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, nonn
 * Formula:   a(n) = the n-th positive integer m such that sum_{k=1}^{36}
 *            1/k^m ≡ 0 (mod 37^3) (37^3 = 50653)
 * Range:     u8 n <= 13, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Modulus 37^3 */
#define MOD 50653U
/* Number of terms in the harmonic sum */
#define K 36U

/* Pre‑computed modular inverses of k (1≤k≤36) modulo MOD */
static uint32_t inv_k[37];
/* Current powers of the inverses: pow_k[i] = (inv_k[i])^exp (mod MOD) */
static uint32_t pow_k[37];
static bool initialized = false;

/* Storage for the sequence values (the exponents m that satisfy the condition) */
static seq_t *matches = NULL;
static size_t matches_cap = 0;
static size_t matches_len = 0;
/* The exponent for which pow_k contains the powers that were just processed */
static idx_t last_exp = 0;

/* Extended‑Euclid inverse for 32‑bit unsigned integers */
static uint32_t modinv_uint32(uint32_t a, uint32_t mod)
{
    int64_t t = 0, newt = 1;
    int64_t r = (int64_t)mod, newr = (int64_t)a;
    while (newr != 0) {
        int64_t q = r / newr;
        int64_t tmp = newt; newt = t - q * newt; t = tmp;
        tmp = newr; newr = r - q * newr; r = tmp;
    }
    if (r > 1) return 0;          /* not invertible */
    if (t < 0) t += (int64_t)mod;
    return (uint32_t)t;
}

/* Ensure we have enough terms stored in the array */
static void ensure_terms(size_t need)
{
    /* First time only: compute inverses and initialise pow_k */
    if (!initialized) {
        for (unsigned int i = 1; i <= K; ++i) {
            inv_k[i] = modinv_uint32((uint32_t)i, MOD);
            pow_k[i] = inv_k[i];          /* for exponent 1 */
        }
        initialized = true;
        last_exp = 0;                    /* nothing processed yet */
    }

    /* Grow the matches array if necessary */
    if (need > matches_cap) {
        size_t new_cap = (matches_cap == 0) ? 32 : matches_cap * 2;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(matches, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* allocation failure */
        matches = tmp;
        matches_cap = new_cap;
    }

    /* Compute terms until we have at least 'need' stored */
    while (matches_len < need) {
        idx_t exp = last_exp + 1;      /* current exponent */
        uint32_t sum = 0;
        for (unsigned int i = 1; i <= K; ++i) {
            sum += pow_k[i];
            if (sum >= MOD) sum -= MOD;
        }
        if (sum == 0) {                /* numerator divisible by 37^3 */
            matches[matches_len++] = (seq_t)exp;
        }

        /* Advance to the next exponent: multiply each power by its base inverse */
        for (unsigned int i = 1; i <= K; ++i) {
            pow_k[i] = (uint32_t)((pow_k[i] * (uint64_t)inv_k[i]) % MOD);
        }
        last_exp = exp;
    }
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0 */
    ensure_terms((size_t)n);
    return matches[n - 1];
}
