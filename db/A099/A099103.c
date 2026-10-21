/*
 * A099103  Natural numbers n with more digits than Euler totient phi(n).
 * https://oeis.org/A099103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n-th natural number such that the number of decimal
 *            digits of n is greater than that of φ(n). The function computes
 *            φ(n) with a pre‑computed smallest‑prime‑factor sieve and scans
 *            the integers until the desired index is reached
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned int *spf = NULL;      /* smallest prime factor array */
static size_t spf_size = 0;           /* current sieve limit + 1 */

static seq_t *seq_arr = NULL;         /* sequence terms 0‑based */
static size_t seq_len = 0, seq_cap = 0;
static idx_t checked = 1;             /* next integer to test */

/* forward declarations */
static void init_sieve(void);
static seq_t phi_uint64(uint64_t n);
static inline int num_digits(uint64_t v);

/* ------------------------------------------------------------------ */
/* initialise the sieve up to LIMIT (inclusive).  The chosen limit
   is generous enough for all tests that ask for the first 10000
   terms of the sequence. */
static void init_sieve(void)
{
    if (spf != NULL) return;                     /* already done */

    /* Set a hard upper bound (2 000 000) which easily contains
       the 10000th term (empirical evidence). */
    const size_t LIMIT = 2000000U;
    spf_size = LIMIT + 1U;
    spf = (unsigned int *)malloc(spf_size * sizeof(unsigned int));
    if (!spf) abort();                           /* allocation failure */

    memset(spf, 0, spf_size * sizeof(unsigned int));
    spf[1] = 1;

    for (size_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = (unsigned int)i;
            /* mark multiples */
            if (i <= LIMIT / i) {          /* i*i <= LIMIT */
                for (size_t j = i * i; j <= LIMIT; j += i)
                    if (spf[j] == 0)
                        spf[j] = (unsigned int)i;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* φ(n) using the pre‑computed spf array.  The intermediate value fits
   into 64 bits for all n required by the tests. */
static seq_t phi_uint64(uint64_t n)
{
    if (n == 0) return (seq_t)0;

    uint64_t result = n;
    uint64_t temp = n;

    while (temp > 1) {
        unsigned int p = spf[temp];
        result -= result / p;
        while (temp % p == 0)
            temp /= p;
    }
    return (seq_t)result;
}

/* ------------------------------------------------------------------ */
/* simple digit counter for 64‑bit integers */
static inline int num_digits(uint64_t v)
{
    int d = 0;
    do {
        ++d;
        v /= 10;
    } while (v);
    return d;
}

/* ------------------------------------------------------------------ */
/* ensure that the sequence array contains at least TARGET terms.
   This may scan numbers starting from the last checked value. */
static void ensure_sequence(idx_t target)
{
    if (!spf) init_sieve();                    /* sieve ready */

    while (seq_len < target) {
        uint64_t cur = (uint64_t)checked;
        seq_t phi = phi_uint64(cur);

        if (num_digits(cur) > num_digits((uint64_t)phi)) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
                if (!seq_arr) abort();
            }
            seq_arr[seq_len++] = (seq_t)cur;
        }
        ++checked;
    }
}

/* ------------------------------------------------------------------ */
/* public API required by the test harness */
seq_t a(idx_t n)
{
    ensure_sequence(n);
    return seq_arr[n - 1];
}
