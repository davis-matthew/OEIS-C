/*
 * A158814  Multiples of 10 in the EKG sequence A064413.
 * https://oeis.org/A158814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: multiples of 10 in the EKG sequence A064413.
   Computed by simulating the EKG sequence (a(1)=1, a(2)=2,
   a(n) = smallest unused integer sharing a non‑trivial
   common factor with a(n‑1)) and collecting every term
   that is divisible by 10. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#define LIMIT 2000000          /* maximum number pre‑computed */

/* static state */
static bool init_done = false;
static idx_t spf[LIMIT + 1];          /* smallest prime factor */
static uint8_t used[LIMIT + 1];       /* 0 = unused, 1 = used */
static idx_t nextp[LIMIT + 1];        /* next unused multiple of a prime */

static seq_t *multiples = NULL;      /* collected multiples of 10 */
static size_t mul_len   = 0;          /* number of multiples stored   */
static size_t mul_cap   = 0;          /* allocated capacity           */

static idx_t ekg_prev = 2;           /* last term processed */
static idx_t ekg_pos  = 2;           /* number of EKG terms generated */

/* initialise arrays and state once */
static void init(void)
{
    if (init_done) return;

    for (idx_t i = 0; i <= LIMIT; ++i) { spf[i] = 0; used[i] = 0; nextp[i] = 0; }

    /* build smallest prime factor and nextp */
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {               /* i is prime */
            spf[i]   = i;
            nextp[i] = i;
            if (i <= LIMIT / i) {
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* first two terms used */
    used[1] = 1;
    used[2] = 1;
    ekg_prev = 2;
    ekg_pos  = 2;

    /* allocate storage for multiples of 10 */
    mul_cap   = 64;
    multiples = (seq_t*)malloc(mul_cap * sizeof(seq_t));

    init_done = true;
}

/* generate the next term of the EKG sequence */
static void ekg_next(void)
{
    /* factorise current term, collect distinct primes */
    idx_t x = ekg_prev;
    idx_t primes[64];
    size_t pcnt = 0;
    idx_t last = 0;

    while (x > 1) {
        idx_t p = spf[x];
        if (p != last) { primes[pcnt++] = p; last = p; }
        x /= p;
    }

    /* choose smallest unused number sharing a prime factor */
    idx_t candidate = (idx_t)-1;
    for (size_t i = 0; i < pcnt; ++i) {
        idx_t p = primes[i];
        idx_t c = nextp[p];
        while (c <= LIMIT && used[c]) c += p;  /* advance to unused */
        nextp[p] = c;
        if (c < candidate) candidate = c;
    }

    idx_t next_term = candidate;
    used[next_term] = 1;                 /* mark used */

    /* update nextp for primes of the new term */
    x = next_term;
    last = 0;
    while (x > 1) {
        idx_t p = spf[x];
        if (p != last) {
            if (nextp[p] == next_term) {
                idx_t t = next_term;
                do { t += p; } while (t <= LIMIT && used[t]);
                nextp[p] = t;
            }
            last = p;
        }
        x /= p;
    }

    ekg_prev = next_term;
    ++ekg_pos;

    /* store the term if it is a multiple of 10 */
    if ((next_term % 10) == 0) {
        if (mul_len == mul_cap) {
            mul_cap *= 2;
            multiples = (seq_t*)realloc(multiples, mul_cap * sizeof(seq_t));
        }
        multiples[mul_len++] = (seq_t)next_term;
    }
}

/* requested OEIS function */
seq_t a(idx_t n)
{
    init();                      /* ensure state is initialised */

    /* generate terms until we have the nth multiple of 10 */
    while (mul_len < n) ekg_next();

    return multiples[n - 1];
}
