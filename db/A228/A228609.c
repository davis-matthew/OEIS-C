/*
 * A228609  Partial sums of the cubes of the tribonacci sequence A000073.
 * https://oeis.org/A228609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + (A000073(n))^3, with tribonacci T(0)=0, T(1)=1,
 *            T(2)=1, a(0)=0, a(1)=1, a(2)=2, ... The function memoizes up to
 *            the largest parameter seen so far
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *trib = NULL;
static seq_t *sums = NULL;
static size_t capacity = 0;
static size_t covered = 0;

static void ensure_cap(size_t new_cap)
{
    if (new_cap <= capacity)
        return;

    seq_t *new_trib = NULL;
    seq_t *new_sums = NULL;

#if defined(__cplusplus)
    new_trib = new seq_t[new_cap];
    new_sums = new seq_t[new_cap];
#else
    new_trib = malloc(new_cap * sizeof(seq_t));
    new_sums = malloc(new_cap * sizeof(seq_t));
    if (!new_trib || !new_sums) exit(1);
#endif

    for (size_t i = 0; i < covered; ++i) {
        new_trib[i] = trib[i];
        new_sums[i] = sums[i];
    }

#if defined(__cplusplus)
    delete[] trib;
    delete[] sums;
#else
    free(trib);
    free(sums);
#endif

    trib = new_trib;
    sums = new_sums;
    capacity = new_cap;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;

    /* Initialise base tribonacci values and partial sums */
    if (trib == NULL) {
        ensure_cap(3);          /* need indices 0,1,2 */
        trib[0] = (seq_t)0;
        trib[1] = (seq_t)1;
        trib[2] = (seq_t)1;

        seq_t cube0 = trib[0] * trib[0] * trib[0];
        sums[0] = cube0;                       /* 0 */

        seq_t cube1 = trib[1] * trib[1] * trib[1];
        sums[1] = sums[0] + cube1;             /* 1 */

        seq_t cube2 = trib[2] * trib[2] * trib[2];
        sums[2] = sums[1] + cube2;             /* 2 */

        covered = 3;
    }

    if (N >= covered) {
        ensure_cap(N + 1);                       /* space for index N */

        for (size_t i = covered; i <= N; ++i) {
            if (i >= 3) {
                trib[i] = trib[i - 1] + trib[i - 2] + trib[i - 3];
            }
            /* i==0,1,2 already set */
            seq_t cube = trib[i] * trib[i] * trib[i];
            sums[i] = sums[i - 1] + cube;
        }
        covered = N + 1;
    }

    return sums[N];
}
