/*
 * A071320  Least of four consecutive numbers which are cubefree and not
 *          squarefree, i.e., numbers k such that {k, k+1, k+2, k+3} are in
 *          A067259.
 * https://oeis.org/A071320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 38, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A071320 – Least of four consecutive cubefree, not squarefree numbers  
    Algorithm:  
    • Compute an array of the smallest prime factors (spf) up to a fixed limit (20 000 000).  
    • Scan integers from 1 upwards. For each integer m check that  
          – m is divisible by a square but not by a cube (i.e. exists a prime p with exponent 2, all exponents ≤2).  
      This is done using the spf array.  
    • Maintain a counter of consecutive qualifying numbers.  
      Whenever the counter reaches 4, the start of the run k = m‑3 is stored.  
    • The sequence terms are cached so that arbitrary‐order calls to a(n) are fast.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *spf = NULL;            /* smallest prime factor table   */
static const size_t MAX_LIMIT = 20000000U; /* pre‑computed sieving limit    */

static void init_spf(void)
{
    if (spf != NULL) return;                     /* already built */
    spf = (uint32_t *)malloc((MAX_LIMIT + 1) * sizeof(uint32_t));
    if (!spf) return;                           /* allocation failure – unlikely in a contest setting */
    memset(spf, 0, (MAX_LIMIT + 1) * sizeof(uint32_t));
    spf[0] = 0; spf[1] = 1;

    for (size_t i = 2; i <= MAX_LIMIT; ++i) {
        if (spf[i] == 0) { /* i is prime */
            spf[i] = (uint32_t)i;
            if (i <= MAX_LIMIT / i) {
                for (size_t j = i * i; j <= MAX_LIMIT; j += i) {
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }
    }
}

/* true iff n is cubefree and not squarefree */
static bool satisfies_one(uint64_t n)
{
    if (n <= 1) return false;
    bool has_square = false;
    uint64_t x = n;
    while (x > 1) {
        uint32_t p = spf[(size_t)x];
        int exp = 0;
        while (x % p == 0) {
            x /= p;
            ++exp;
            if (exp >= 3) return false;  /* cube factor detected */
        }
        if (exp == 2) has_square = true;
    }
    return has_square;  /* must contain a square factor */
}

/* dynamic storage for sequence terms */
static seq_t *terms = NULL;
static size_t terms_len = 0, terms_cap = 0;

static void push_term(seq_t v)
{
    if (terms_len == terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 8;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_len++] = v;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0, but harmless */
    init_spf();

    static uint64_t scan = 1;        /* next integer to test */
    static uint32_t consec = 0;      /* consecutive qualifying numbers */

    while (terms_len < n) {
        if (scan > MAX_LIMIT) {
            /* The pre‑computed limit is exceeded.  In the intended range this
               should never happen.  If it does, we simply stop – behaviour
               is undefined for n beyond the limit. */
            break;
        }

        bool ok = satisfies_one(scan);
        if (ok) {
            ++consec;
        } else {
            consec = 0;
        }

        if (consec >= 4) {
            seq_t k = (seq_t)(scan - 3);   /* start of current run of 4 */
            push_term(k);
        }
        ++scan;
    }

    return terms[n - 1];
}
