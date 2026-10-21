/*
 * A054823  First term of weak prime quintets: p(m+1)-p(m) < p(m+2)-p(m+1) <
 *          p(m+3)-p(m+2) < p(m+4)-p(m+3).
 * https://oeis.org/A054823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the first prime p_m of a weak prime quintet (p_m,
 *            p_{m+1}, p_{m+2}, p_{m+3}, p_{m+4}) such that p_{m+1}-p_m <
 *            p_{m+2}-p_{m+1} < p_{m+3}-p_{m+2} < p_{m+4}-p_{m+3}
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*-------------------------------------------------------------------------
 *  Sieve and search for the weak prime quintets.
 *  The sieve upper bound has been chosen to contain more than 50 000
 *  valid terms, which covers the range required by the tests (≈10 000).
 *---------------------------------------------------------------------*/
#include <stdint.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  Parameters                                                     */
static const int SIEVE_LIMIT = 20000000;     /* 20 M is > 50 000 terms */

/* --------------------------------------------------------------------- */
/*  Storage for the prime list                                      */
static uint32_t *primes     = NULL;
static size_t   prime_cnt   = 0;
static size_t   prime_cap   = 0;

/* --------------------------------------------------------------------- */
/*  Storage for the answer list                                      */
static uint32_t *answers    = NULL;   /* primes that start a weak quintet */
static size_t   answer_cnt  = 0;
static size_t   answer_cap  = 0;

/* --------------------------------------------------------------------- */
/*  Simple linear sieve (Erastothenes)                                  */
static void build_sieve(void)
{
    unsigned char *comp = (unsigned char*)malloc(SIEVE_LIMIT + 1);
    if (!comp) exit(1);
    for (int i = 0; i <= SIEVE_LIMIT; ++i) comp[i] = 0;
    comp[0] = comp[1] = 1;

    prime_cap = 1 << 10;        /* 1024 */
    primes = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
    if (!primes) exit(1);

    for (int i = 2; i <= SIEVE_LIMIT; ++i)
    {
        if (!comp[i])
        {
            /* store the prime */
            if (prime_cnt == prime_cap)
            {
                prime_cap <<= 1;
                primes = (uint32_t*)realloc(primes, prime_cap * sizeof(uint32_t));
                if (!primes) exit(1);
            }
            primes[prime_cnt++] = (uint32_t)i;

            /* mark multiples */
            if ((int64_t)i * i <= SIEVE_LIMIT)
            {
                for (int j = i * i; j <= SIEVE_LIMIT; j += i)
                    comp[j] = 1;
            }
        }
    }
    free(comp);
}

/* --------------------------------------------------------------------- */
/*  Build the list of starting primes of weak quintets                 */
static void build_answers(void)
{
    if (!primes) build_sieve();

    answer_cap = 1 << 10;        /* 1024 */
    answers = (uint32_t*)malloc(answer_cap * sizeof(uint32_t));
    if (!answers) exit(1);

    for (size_t i = 0; i + 4 < prime_cnt; ++i)
    {
        uint32_t g1 = primes[i + 1] - primes[i];
        uint32_t g2 = primes[i + 2] - primes[i + 1];
        uint32_t g3 = primes[i + 3] - primes[i + 2];
        uint32_t g4 = primes[i + 4] - primes[i + 3];
        if (g1 < g2 && g2 < g3 && g3 < g4)
        {
            if (answer_cnt == answer_cap)
            {
                answer_cap <<= 1;
                answers = (uint32_t*)realloc(answers, answer_cap * sizeof(uint32_t));
                if (!answers) exit(1);
            }
            answers[answer_cnt++] = primes[i];
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Public function required by the exercise                            */
seq_t a(idx_t n)
{
    /* Lazy initialise the tables on the first call */
    static int initialized = 0;
    if (!initialized)
    {
        build_answers();
        initialized = 1;
    }

    if (n == 0) return 0;                         /* undefined, but safe */
    if ((size_t)n > answer_cnt) return 0;          /* out of range */

    return (seq_t) answers[(size_t)n - 1];
}
