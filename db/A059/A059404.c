/*
 * A059404  Numbers with different exponents in their prime factorizations.
 * https://oeis.org/A059404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A059404: Numbers with different exponents in their prime
   factorizations.  A(n) is the n‑th integer k>1 such that
   k has at least two distinct prime factors and the exponents in the
   factorisation are not all equal (i.e. (k = p1^e1 · … · pr^er,
   r>1, not all ei are the same). */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* ----------------------------------------------------------------------
   Prime sieve – generates all primes up to MAX_PRIME once.
   ---------------------------------------------------------------------- */
#define MAX_PRIME 2000000       /* 148933 primes – more than enough for n≤10 000 */
static int primes[MAX_PRIME / 2 + 1];
static size_t prime_cnt = 0;
static bool primes_inited = false;

static void init_primes (void)
{
    if (primes_inited) return;
    bool *is_comp = (bool *)calloc((MAX_PRIME + 1), sizeof(bool));
    for (int i = 2; i <= MAX_PRIME; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = i;
            if ((long long)i * i <= MAX_PRIME) {
                for (long long j = (long long)i * i; j <= MAX_PRIME; j += i)
                    is_comp[(size_t)j] = true;
            }
        }
    }
    free(is_comp);
    primes_inited = true;
}

/* ----------------------------------------------------------------------
   Test whether a number k satisfies the property.  The function uses
   trial division with the pre‑computed primes.
   ---------------------------------------------------------------------- */
static bool qualifies (seq_t k)
{
    if (k < 2) return false;
    seq_t tmp = k;
    size_t distinct = 0;
    int firstExp = 0;
    bool allSame = true;

    for (size_t i = 0; i < prime_cnt && (seq_t)primes[i] * primes[i] <= tmp; ++i) {
        if (tmp % primes[i] == 0) {
            ++distinct;
            int e = 0;
            while (tmp % primes[i] == 0) {
                tmp /= primes[i];
                ++e;
            }
            if (distinct == 1) firstExp = e;
            else if (e != firstExp) allSame = false;
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        ++distinct;
        int e = 1;
        if (distinct == 1) firstExp = e;
        else if (e != firstExp) allSame = false;
    }
    if (distinct < 2)  return false;   /* need at least two distinct primes   */
    return !allSame;                    /* all exponents must not be equal   */
}

/* ----------------------------------------------------------------------
   Storage for memoised sequence values.
   ---------------------------------------------------------------------- */
static seq_t *seq_vals   = NULL;
static size_t seq_len    = 0;     /* number of values currently stored   */
static size_t seq_cap    = 0;     /* current capacity of seq_vals array   */
static seq_t last_checked = 1;   /* last integer that was examined     */

static void ensure_capacity (size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (seq_vals) {
        memcpy(new_arr, seq_vals, seq_len * sizeof(seq_t));
        free(seq_vals);
    }
    seq_vals = new_arr;
    seq_cap  = new_cap;
}

/* ----------------------------------------------------------------------
   Main sequence function.
   ---------------------------------------------------------------------- */
seq_t a (idx_t n)
{
    if (n == 0) return 0;       /* undefined – harmless             */

    init_primes();              /* ensure that primes are ready      */

    /* Return cached value if already known */
    if ((size_t)n <= seq_len)
        return seq_vals[(size_t)n - 1];

    /* Grow the array to at least n elements */
    ensure_capacity((size_t)n);

    /* Generate subsequent integers until enough terms are found */
    for (;;) {
        last_checked = last_checked + (seq_t)1;
        if (qualifies(last_checked)) {
            seq_vals[seq_len++] = last_checked;
            if (seq_len == (size_t)n)         /* desired term obtained */
                return seq_vals[seq_len - 1];
        }
    }
}
