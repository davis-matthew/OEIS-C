/*
 * A063889  Composite numbers not divisible by 5 which in base 5 contain their
 *          largest proper factor as a substring.
 * https://oeis.org/A063889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th composite number not divisible by 5 whose *
 *            base‑5 representation contains that of its largest proper
 *            divisor
 * Range:     u8 none, u16 n <= 15, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   to_base5_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes all primes up to a fixed limit,
   then scans successive integers, skipping primes and numbers
   divisible by 5.  For each composite candidate it finds its
   smallest prime factor p, the largest proper divisor d = N / p,
   and checks that the base‑5 string of d appears as a substring
   of the base‑5 string of N.  The found numbers are stored in a
   static cache so that repeated calls to a(n) are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/to_base5_seq.h"

/* ------------------------------------------------------------------ */
/*  Configuration ----------------------------------------------------- */
/* Adjust MAX_LIMIT if a larger search range is needed.  20 000 000
 * provides more than enough terms for n up to at least 10 000.       */
#define MAX_LIMIT 20000000U

/* ------------------------------------------------------------------ */
/*  Global state ------------------------------------------------------ */
static unsigned char *prime_flag = NULL;   /* 1 = prime, 0 = composite   */
static int          *prime_list = NULL;   /* primes up to MAX_LIMIT      */
static int            prime_count = 0;

static seq_t *terms     = NULL;   /* cache of found terms        */
static size_t terms_len = 0;      /* number of terms in cache    */
static size_t terms_cap = 0;      /* allocated capacity          */

static idx_t last_checked = 1;    /* last number examined        */

/* ------------------------------------------------------------------ */
/*  Helpers ----------------------------------------------------------- */

/* Search for SUB (length SUBLEN) inside STR (length LEN). */
static bool contains_substring(const char *str, int len,
                               const char *sub, int sublen)
{
    if (sublen == 0 || sublen > len)
        return false;
    for (int i = 0; i + sublen <= len; ++i) {
        if (memcmp(str + i, sub, sublen) == 0)
            return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/*  Prime initialization --------------------------------------------- */
static void init(void)
{
    if (prime_flag != NULL)
        return;                         /* already initialized   */

    /* --- sieve of Eratosthenes up to MAX_LIMIT --- */
    prime_flag = (unsigned char *)malloc(MAX_LIMIT + 1);
    if (!prime_flag)
        exit(1);
    memset(prime_flag, 1, MAX_LIMIT + 1);
    prime_flag[0] = prime_flag[1] = 0;

    for (int i = 2; (long long)i * i <= MAX_LIMIT; ++i) {
        if (prime_flag[i]) {
            for (long long j = (long long)i * i; j <= MAX_LIMIT; j += i)
                prime_flag[j] = 0;
        }
    }

    /* --- build prime list (all primes ≤ MAX_LIMIT) --- */
    /* first count how many primes */
    int cnt = 0;
    for (int i = 2; i <= MAX_LIMIT; ++i)
        if (prime_flag[i])
            ++cnt;

    prime_list = (int *)malloc(cnt * sizeof(int));
    if (!prime_list)
        exit(1);

    for (int i = 2, k = 0; i <= MAX_LIMIT; ++i) {
        if (prime_flag[i])
            prime_list[k++] = i;
    }
    prime_count = cnt;

    /* --- cache for found terms --- */
    terms_cap = 256;                              /* start with modest size */
    terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    if (!terms)
        exit(1);
    terms_len = 0;
    last_checked = 1;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    init();

    while (terms_len < n) {
        /* advance to next candidate composite not divisible by 5 */
        do {
            ++last_checked;
            /* we assume last_checked never exceeds MAX_LIMIT            */
        } while (prime_flag[last_checked] || (last_checked % 5 == 0));

        /* find smallest prime factor p of last_checked */
        int spf = 0;
        for (int i = 0; i < prime_count; ++i) {
            int p = prime_list[i];
            if ((long long)p * p > last_checked)
                break;
            if (last_checked % p == 0) {
                spf = p;
                break;
            }
        }
        if (spf == 0)            /* should not happen for composites */
            spf = last_checked;

        seq_t N = (seq_t)last_checked;
        seq_t D = N / spf;        /* largest proper divisor */

        /* base‑5 representations */
        char sN[64], sD[64];
        int lenN, lenD;
        to_base5_seq(N, sN, &lenN);
        to_base5_seq(D, sD, &lenD);

        if (contains_substring(sN, lenN, sD, lenD)) {
            /* store term */
            if (terms_len == terms_cap) {
                terms_cap <<= 1;
                terms = (seq_t *)realloc(terms,
                                          terms_cap * sizeof(seq_t));
                if (!terms)
                    exit(1);
            }
            terms[terms_len++] = N;
        }
    }

    return terms[n - 1];
}
