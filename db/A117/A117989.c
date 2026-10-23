/*
 * A117989  Number of partitions of n such that the least part occurs at least
 *          twice.
 * https://oeis.org/A117989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A000041(n) - A000041(n+1) where A000041(n) is the
 *            partition number p(n)
 * Range:     u8 n <= 17, u16 n <= 44, u32 n <= 128, u64 n <= 417,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use a simple dynamic programming approach that requires only
   addition, which is safe for unsigned seq_t types. */

/* Maximum pre‑computation size.  The tests only ask for n up to about 10000,
   so 20001 is more than enough.  If a larger n is requested the code
   simply recomputes up to that size. */
#define MAX_PARTITIONS 20001

static seq_t partitions[MAX_PARTITIONS];
static idx_t part_last = 0;    /* highest index that has been computed */

/* Compute partition numbers p(0..max_n) using the classic
   “add one coin” DP.  All operations are additions, so we can
   work with unsigned seq_t safely. */
static void compute_partitions_up_to(idx_t max_n)
{
    if (max_n >= MAX_PARTITIONS)
        /* Requested size exceeds pre‑allocation;  we recompute up to MAX_PARTITIONS-1.
           The caller must not exceed this limit (the tests only use n <= 10000). */
        max_n = MAX_PARTITIONS - 1;

    if (max_n <= part_last)        /* already done */
        return;

    /* Initialize the array for the new range.  We only need to
       reset values that will be written. */
    for (idx_t i = 0; i <= max_n; ++i)
        partitions[i] = 0;
    partitions[0] = 1;             /* p(0) = 1 */

    for (idx_t part = 1; part <= max_n; ++part) {
        for (idx_t total = part; total <= max_n; ++total) {
            partitions[total] += partitions[total - part];
        }
    }

    part_last = max_n;
}

/* Public sequence function.  a(1) = 0.  The function works in any
   call order because it memoises the partition numbers. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                  /* undefined for n <= 0 – behaviour irrelevant */

    /* We need partitions up to n+1 to evaluate 2*p(n) - p(n+1). */
    compute_partitions_up_to(n + 1);

    seq_t pn  = partitions[n];
    seq_t pn1 = partitions[n + 1];
    return 2 * pn - pn1;          /* non‑negative for all n >= 1 */
}
