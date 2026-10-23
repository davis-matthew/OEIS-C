/*
 * A175509  a(n) = ((2*n)-th semiprime) minus 2*(n-th semiprime).
 * https://oeis.org/A175509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ((2*n)-th semiprime) – 2*(n-th semiprime)
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------------------------------------------------------------------- */
/*  Global data ---------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
static uint32_t *spf      = NULL;  /* smallest prime factor table */
static idx_t     spf_limit = 0;    /* current size of spf array      */

static seq_t *semip      = NULL;  /* array of semiprimes            */
static size_t semip_cap  = 0;      /* current capacity of semip      */
static idx_t  semip_cnt  = 0;      /* number of stored semiprimes    */

/* ---------------------------------------------------------------------- */
/*  Helper: build spf up to 'limit'.  Returns true on success.           */
/* ---------------------------------------------------------------------- */
static bool build_spf(idx_t limit)
{
    uint32_t *new_spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!new_spf) return false;
    for (idx_t i = 0; i <= limit; ++i) new_spf[i] = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = (uint32_t)i;
            if (i > limit / i) continue;   /* avoid overflow */
            for (idx_t j = i * i; j <= limit; j += i) {
                if (new_spf[j] == 0) new_spf[j] = (uint32_t)i;
            }
        }
    }

    if (spf) free(spf);
    spf = new_spf;
    spf_limit = limit;
    return true;
}

/* ---------------------------------------------------------------------- */
/*  Helper: generate semiprimes up to 'limit'.  Store at least 'need'.  */
/* ---------------------------------------------------------------------- */
static bool generate_semiprimes(idx_t need)
{
    if (!build_spf(spf_limit)) return false;

    /* free old list (if any) and prepare for new */
    if (semip) free(semip);
    semip_cap = need + 16;                    /* start with some room   */
    semip     = (seq_t *)malloc(semip_cap * sizeof(seq_t));
    if (!semip) return false;
    semip_cnt = 0;

    for (idx_t x = 4; x <= spf_limit; ++x) {
        uint32_t p = spf[x];
        if (p == 0 || p == (uint32_t)x) continue;   /* not composite */
        idx_t m = x / p;
        if (m >= 2 && spf[m] == (uint32_t)m) {      /* m is prime */
            if (semip_cnt >= semip_cap) {
                semip_cap <<= 1;                    /* grow */
                seq_t *tmp = (seq_t *)realloc(semip, semip_cap * sizeof(seq_t));
                if (!tmp) return false;
                semip = tmp;
            }
            semip[semip_cnt++] = (seq_t)x;
            if (semip_cnt >= need) break;
        }
    }
    return semip_cnt >= need;
}

/* ---------------------------------------------------------------------- */
/*  Init --------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
static void init_sequence(void)
{
    /* First time: sieve to a conservative limit that covers n up to 20000. */
    if (spf) return;                /* already done */

    /* Choose limit: 1 000 000 gives >190 000 semiprimes, enough for n≤20000. */
    spf_limit = 1000000;
    build_spf(spf_limit);

    /* Build semiprime list for the first 20000 required semiprimes. */
    generate_semiprimes(20000);     /* at least 20000 semiprimes will exist */
}

/* ---------------------------------------------------------------------- */
/*  Main sequence function ------------------------------------------------ */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 1, safe sentinel */

    init_sequence();                /* prime sieve & first batch of semiprimes */

    /* If requested semiprimes exceed precomputed ones, extend. */
    idx_t needed = 2 * n;            /* need up to (2n)-th semiprime */
    if (needed > semip_cnt) {
        /* Keep enlarging the sieve until we have at least 'needed' semiprimes. */
        while (needed > semip_cnt) {
            spf_limit <<= 1;                               /* double size   */
            if (!generate_semiprimes(needed))             /* rebuild list   */
                break;                                   /* fatal, abort later */
        }
    }

    seq_t sp1 = semip[n - 1];          /* n-th semiprime  (0‑based index) */
    seq_t sp2 = semip[2 * n - 1];      /* (2n)-th semiprime */
    return sp2 - (seq_t)2 * sp1;
}
