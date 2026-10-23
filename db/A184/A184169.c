/*
 * A184169  Maximum escape distance over the vertices of the rooted tree
 *          having Matula-Goebel number n.
 * https://oeis.org/A184169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum escape distance of vertices in the rooted tree
 *            whose Matula–Goebel number equals n
 * Range:     u8 n <= 110, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Prime table – built lazily on first use. */
static idx_t *primeArr = NULL;
static size_t primeCnt = 0;

static void init_primes(void)
{
    if (primeArr != NULL)
        return;                       /* already built */

    /* Generate all primes up to 20000 – this covers every prime factor
       that can ever appear while computing a(n) for n <= 10000. */
    const idx_t LIMIT = 20000;
    bool *composite = (bool *)malloc((size_t)(LIMIT + 1) * sizeof(bool));
    if (!composite)
        return;
    memset(composite, 0, (size_t)(LIMIT + 1) * sizeof(bool));

    /* Allocate room for a safe upper bound on the number of primes. */
    primeArr = (idx_t *)malloc(2500 * sizeof(idx_t));
    if (!primeArr) {
        free(composite);
        return;
    }
    primeCnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (!composite[i]) {
            primeArr[primeCnt++] = i;
            if (i * i <= LIMIT) {
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    composite[j] = true;
            }
        }
    }
    free(composite);
}

/* Result of a recursive descent: minimal escape distance from the root
   of the subtree, and the maximal escape distance over all vertices
   of that subtree. */
typedef struct {
    seq_t minDist;
    seq_t maxDist;
} DistRec;

/* Recurse on the tree having Matula–Goebel number n. */
static DistRec dfs(idx_t n)
{
    if (n == 1)                      /* single vertex */
        return (DistRec){(seq_t)0, (seq_t)0};

    init_primes();

    idx_t tmp = n;
    seq_t minChildDist = 0;
    seq_t childMaxDist = 0;
    bool firstChild = true;

    for (size_t i = 0; i < primeCnt; ++i) {
        idx_t p = primeArr[i];
        if (p * p > tmp)
            break;
        while (tmp % p == 0) {
            idx_t childMatula = (idx_t)(i + 1);   /* prime indices are 1‑based */
            DistRec child = dfs(childMatula);
            seq_t cMin = child.minDist;
            seq_t cMax = child.maxDist;

            if (firstChild) {
                minChildDist = cMin;
                firstChild  = false;
            } else if (cMin < minChildDist) {
                minChildDist = cMin;
            }

            if (cMax > childMaxDist)
                childMaxDist = cMax;

            tmp /= p;
        }
    }

    if (tmp > 1) {                      /* remaining prime factor */
        /* binary search for its index in the prime array */
        size_t lo = 0, hi = primeCnt;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (primeArr[mid] < tmp)
                lo = mid + 1;
            else
                hi = mid;
        }
        idx_t childMatula = (idx_t)(lo + 1);
        DistRec child = dfs(childMatula);
        seq_t cMin = child.minDist;
        seq_t cMax = child.maxDist;

        if (firstChild) {
            minChildDist = cMin;
            firstChild  = false;
        } else if (cMin < minChildDist) {
            minChildDist = cMin;
        }

        if (cMax > childMaxDist)
            childMaxDist = cMax;
    }

    seq_t rootMinDist = minChildDist + (seq_t)1;
    seq_t rootMaxDist = rootMinDist;
    if (childMaxDist > rootMaxDist)
        rootMaxDist = childMaxDist;

    return (DistRec){rootMinDist, rootMaxDist};
}

seq_t a(idx_t n)
{
    DistRec res = dfs(n);
    return res.maxDist;
}
