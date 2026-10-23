/*
 * A118781  Determinants of 3 X 3 matrices of continuous blocks of 9
 *          consecutive semiprimes.
 * https://oeis.org/A118781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = s(n)*s(n+4)*s(n+8) * - s(n)*s(n+5)*s(n+7) * -
 *            s(n+1)*s(n+3)*s(n+8) * + s(n+1)*s(n+5)*s(n+6) * +
 *            s(n+2)*s(n+3)*s(n+7) * - s(n+2)*s(n+4)*s(n+6) * * where s(k) =
 *            the k-th semiprime (OEIS A001358). The sequence begins * with
 *            a(1) = -196, a(2) = 272, … . * * This implementation builds a
 *            dynamic...
 * Range:     i8 none, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free, NULL */
#include <stdint.h>   /* uint32_t */
#include <stdbool.h>  /* bool */

static seq_t *semiprimes = NULL;   /* array of semiprimes (0‑based for s(1)) */
static idx_t semip_size   = 0;      /* number of generated semiprimes */
static idx_t semip_cap    = 0;      /* current capacity of semiprimes array */

static uint32_t *spf      = NULL;   /* smallest prime factor array */
static idx_t spf_limit    = 0;       /* current maximum index of spf array */

static idx_t next_check  = 4;       /* next integer to inspect for semiprime */

/* ---- smallest prime factor sieve ------------------------------------- */
static void ensure_spf_up_to(idx_t limit)
{
    if (limit <= spf_limit)
        return;

    idx_t new_limit = limit;
    uint32_t *new_spf = (uint32_t *)realloc(spf, sizeof(uint32_t) * (new_limit + 1));
    spf = new_spf;
    spf_limit = new_limit;

    /* initialise array to 0 */
    for (idx_t i = 0; i <= new_limit; ++i)
        spf[i] = 0;

    /* sieve */
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = (uint32_t)i;
            if ((uint64_t)i * i <= new_limit) {
                for (idx_t j = i * i; j <= new_limit; j += i)
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
            }
        }
    }
}

/* ---- ensure we have at least 'need' semiprimes ------------------------ */
static void ensure_semiprimes(idx_t need)
{
    if (semip_size >= need)
        return;

    /* enlarge array if needed */
    if (semip_cap < need) {
        idx_t new_cap = semip_cap ? semip_cap * 2 : 128;
        if (new_cap < need)
            new_cap = need;
        semiprimes = (seq_t *)realloc(semiprimes, sizeof(seq_t) * new_cap);
        semip_cap = new_cap;
    }

    /* generate semiprimes until we reach 'need' */
    while (semip_size < need) {
        /* ensure spf information is available for the next candidate */
        if (next_check > spf_limit) {
            /* grow the sieve area; double to amortise work */
            idx_t new_spf_limit = next_check * 2;
            if (new_spf_limit > next_check + 1000)
                new_spf_limit = next_check + 1000;
            ensure_spf_up_to(new_spf_limit);
        }

        uint32_t p = spf[next_check];
        if (p != 0 && p != next_check) {          /* composite */
            uint32_t q = next_check / p;
            if (q >= 2 && spf[q] == q) {         /* q is prime => semiprime */
                semiprimes[semip_size++] = (seq_t)next_check;
            }
        }
        ++next_check;
    }
}

/* ---- main sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for n < 1, return 0 as a safe value */
        return 0;

    idx_t needed = n + 8;           /* we need s(n) .. s(n+8) */
    ensure_semiprimes(needed);

    seq_t *s = semiprimes;          /* alias for clarity */

    seq_t s0 = s[n - 1];
    seq_t s1 = s[n];
    seq_t s2 = s[n + 1];
    seq_t s3 = s[n + 2];
    seq_t s4 = s[n + 3];
    seq_t s5 = s[n + 4];
    seq_t s6 = s[n + 5];
    seq_t s7 = s[n + 6];
    seq_t s8 = s[n + 7];

    seq_t term =  s0 * s4 * s8
                - s0 * s5 * s7
                - s1 * s3 * s8
                + s1 * s5 * s6
                + s2 * s3 * s7
                - s2 * s4 * s6;
    return term;
}
