/*
 * A075791  If n can be written in the form r^i-s^j (see A074981), where
 *          r,s,i,j are integers with r>0, s>0, i>1, j>1 choose the
 *          representation with smallest r^i (in case of ties, minimize i and
 *          j); or if n is not of this form set r=s=i=j=0; sequence gives
 *          values of j.
 * https://oeis.org/A075791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 29, u32 n <= 105, u64 n <= 105, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A075791
   a(n) = exponent j in the minimal representation n = r^i – s^j
   (r>0, s>0, i>1, j>1).  If no such representation exists a(n)=0.
   The implementation pre‑computes all perfect powers up to 10^12,
   keeps the minimal exponent for each value, and scans the sorted list
   for the first r^i > n that yields a perfect‑power difference.  A
   special case handles the diff == 1 case (j = 2). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/*  Helper structure and global tables                       */
/* ------------------------------------------------------------ */

typedef struct { seq_t val; unsigned int exp; } power_t;

static power_t *powers   = NULL;   /* sorted unique perfect powers */
static size_t   power_cnt = 0;     /* number of entries */

/* ------------------------------------------------------------ */
/*  Simple comparison for qsort                                 */
/* ------------------------------------------------------------ */

static int cmp_power(const void *a, const void *b)
{
    const power_t *pa = (const power_t *)a;
    const power_t *pb = (const power_t *)b;
    if (pa->val < pb->val) return -1;
    if (pa->val > pb->val) return 1;
    return 0;
}

/* ------------------------------------------------------------ */
/*  Binary search for exponent of a given perfect power value   */
/* ------------------------------------------------------------ */

static unsigned int power_exponent(seq_t d)
{
    if (d == 1)                /* 1 = 1^j, minimal j is 2 */
        return 2;
    size_t lo = 0, hi = power_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (powers[mid].val < d)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < power_cnt && powers[lo].val == d)
        return powers[lo].exp;
    return 0;
}

/* ------------------------------------------------------------ */
/*  Find first index with powers[i].val > x                    */
/* ------------------------------------------------------------ */

static size_t lower_bound_gt(seq_t x)
{
    size_t lo = 0, hi = power_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (powers[mid].val <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* ------------------------------------------------------------ */
/*  Pre‑compute all perfect powers up to LIMIT64               */
/* ------------------------------------------------------------ */

static void init_powers(void)
{
    if (powers) return;   /* already initialized */

    /* Upper bound for perfect powers (fits in 64‑bit) */
    const uint64_t LIMIT64 = 1000000000000ULL;   /* 10^12 */

    /* Estimate capacity, will grow as needed */
    size_t capacity = 1200000;
    power_t *tmp = (power_t *)malloc(capacity * sizeof(power_t));
    if (!tmp) exit(1);
    size_t cnt = 0;

    /* Generate all powers base^exp, exp >= 2, value <= LIMIT64 */
    for (seq_t base = 2; base * base <= (seq_t)LIMIT64; ++base) {
        seq_t pow = base * base;        /* base^2 */
        unsigned int exp = 2;
        while (pow <= (seq_t)LIMIT64) {
            if (cnt == capacity) {
                capacity <<= 1;
                tmp = (power_t *)realloc(tmp, capacity * sizeof(power_t));
                if (!tmp) exit(1);
            }
            tmp[cnt].val = pow;
            tmp[cnt].exp = exp;
            ++cnt;

            if (pow > (seq_t)LIMIT64 / base)  /* next multiplication would overflow LIMIT */
                break;
            pow *= base;
            ++exp;
        }
    }

    /* Sort all values */
    qsort(tmp, cnt, sizeof(power_t), cmp_power);

    /* Deduplicate keeping the minimal exponent for each value */
    size_t uniq = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (uniq == 0 || tmp[i].val != tmp[uniq - 1].val) {
            tmp[uniq] = tmp[i];
            ++uniq;
        } else {
            /* same value; keep the smaller exponent */
            if (tmp[i].exp < tmp[uniq - 1].exp)
                tmp[uniq - 1].exp = tmp[i].exp;
        }
    }

    /* Allocate final array including 1 as a perfect power (j=2) */
    power_t *final = (power_t *)malloc((uniq + 1) * sizeof(power_t));
    if (!final) exit(1);

    final[0].val = 1;
    final[0].exp = 2;                 /* 1 = 1^2 */
    for (size_t i = 0; i < uniq; ++i)
        final[i + 1] = tmp[i];

    free(tmp);

    powers   = final;
    power_cnt = uniq + 1;
}

/* ------------------------------------------------------------ */
/*  Sequence function                                            */
/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    init_powers();                   /* ensure powers table is ready */

    seq_t N = (seq_t)n;
    size_t idx = lower_bound_gt(N);  /* first perfect power strictly greater than N */

    for (size_t i = idx; i < power_cnt; ++i) {
        seq_t diff = powers[i].val - N;
        if (diff == 0) continue;     /* not a valid difference */
        unsigned int j = power_exponent(diff);
        if (j)                       /* representation found */
            return (seq_t)j;
    }
    return (seq_t)0;                 /* no representation */
}
