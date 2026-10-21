/*
 * A057473  Let prime(i) = i-th prime, let twin(n) = (P,Q) be n-th pair of
 *          twin primes; sequence gives prime(Q).
 * https://oeis.org/A057473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = prime(Q) where Q is the larger prime in the nth pair of
 *            twin primes
 * Range:     u8 n <= 6, u16 n <= 150, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* lazy static state ----------------------------------------------------- */
static seq_t *primes      = NULL;  /* 0‑indexed list of primes          */
static idx_t  primes_cnt  = 0;     /* number of primes stored           */

static seq_t *twin_Q      = NULL;  /* 0‑indexed list of larger twin primes (i.e. Q) */
static idx_t  twin_cnt    = 0;     /* number of twin pairs found         */

static void init(void)
{
    if (primes != NULL)  /* already initialised */
        return;

    /* ------------------------------------------------------------------ */
    /* Set a sieve limit that is safe for at least 10 000 twin prime pairs */
    /* 30 million yields ~1.8 million primes, sufficient for all Q up to  */
    /* about 1.3 million (the 10 000th twin).                               */
    /* ------------------------------------------------------------------ */
    const idx_t LIMIT = 30000000ULL;

    /* allocate composite bitset -------------------------------------- */
    size_t  comp_byte_len = (LIMIT >> 3) + 1;
    unsigned char *comp = (unsigned char*)calloc(comp_byte_len, 1);
    if (!comp) exit(1);

    /* pre‑allocate primes ------------------------------------------------ */
    idx_t primes_cap = 2000000;  /* rough upper bound */
    primes = (seq_t*)malloc(primes_cap * sizeof(seq_t));
    if (!primes) exit(1);
    primes_cnt = 0;

    /* pre‑allocate twin list -------------------------------------------- */
    idx_t twin_cap = 20000;
    twin_Q = (seq_t*)malloc(twin_cap * sizeof(seq_t));
    if (!twin_Q) exit(1);
    twin_cnt = 0;

    /* sieve ------------------------------------------------------------- */
    idx_t prev_prime = 0;  /* numeric value of previous prime */

    for (idx_t i = 2; i <= LIMIT; ++i)
    {
        /* if i is composite jump */
        if ((comp[i >> 3] >> (i & 7)) & 1)
            continue;

        /* store prime -------------------------------------------------- */
        if (primes_cnt == primes_cap) {
            primes_cap <<= 1;
            primes = (seq_t*)realloc(primes, primes_cap * sizeof(seq_t));
            if (!primes) exit(1);
        }
        primes[primes_cnt++] = (seq_t)i;

        /* detect twin pair --------------------------------------------- */
        if (prev_prime && (i - prev_prime == 2))
        {
            if (twin_cnt == twin_cap) {
                twin_cap <<= 1;
                twin_Q = (seq_t*)realloc(twin_Q, twin_cap * sizeof(seq_t));
                if (!twin_Q) exit(1);
            }
            twin_Q[twin_cnt++] = (seq_t)i;   /* larger twin prime = Q */
        }
        prev_prime = i;

        /* mark multiples ------------------------------------------------ */
        if (i * i <= LIMIT)
        {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                comp[j >> 3] |= 1 << (j & 7);
        }
    }

    /* free composite bitset --------------------------------------------- */
    free(comp);
    comp = NULL;
}

/* ---------------------------------------------------------------------- */
/* Return the nth term of A057473.  n starts from 1.                       */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    init();

    if (n > twin_cnt)          /* out of pre‑computed range */
        return 0;

    seq_t Q = twin_Q[n - 1];               /* larger twin prime */
    idx_t idx = (idx_t)(Q - 1);             /* prime index (1‑based) */

    if (idx >= primes_cnt)          /* safety check */
        return 0;

    return primes[idx];
}
